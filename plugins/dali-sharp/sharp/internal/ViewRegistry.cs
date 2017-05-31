﻿using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;

namespace Dali
{
  /// <summary>
  /// Add this attribute to any property belonging to a View (control) you want to be scriptable from JSON
  /// </summary>
  /// <remarks>
  /// Example:
  ///
  /// class MyView : public CustomView
  /// {
  ///  [ScriptableProperty()]
  ///  public int MyProperty
  ///  {
  ///   get
  ///   {
  ///     return _myProperty;
  ///   }
  ///   set
  ///   {
  ///    _myProperty = value;
  ///   }
  ///  }
  /// }
  ///
  /// Internally the following occurs for property registration ( this only occurs once per Type, not per Instance):
  ///
  /// - The controls static constructor should call ViewRegistry.Register()  (only called once for the lifecycle of the app)
  /// - Within Register() the code will introspect the Controls properties, looking for the ScriptableProperty() attribute
  /// - For every property with the ScriptableProperty() attribute, TypeRegistration.RegisterProperty is called.
  /// - TypeRegistration.RegisterProperty calls in to DALi C++ Code Dali::CSharpTypeRegistry::RegisterProperty()
  /// - DALi C++ now knows the existance of the property and will try calling SetProperty, if it finds the property in a JSON file (loaded using builder).
  ///
  ///  The DALi C# example
  ///
  ///  class MyView : public CustomView
  ///  {
  ///
  ///    [ScriptableProperty()]
  ///    public double Hours
  ///    {
  ///     get { return seconds / 3600; }
  ///     set { seconds = value * 3600; }
  ///    }
  ///  }
  ///
  ///  Equivalent code in DALi C++:
  ///  in MyControl.h
  ///  class MyControl : public Control
  ///  {
  ///    struct Property
  ///    {
  ///      enum
  ///      {
  ///        HOURS =  Control::CONTROL_PROPERTY_END_INDEX + 1
  ///      }
  ///    }
  ///  }
  ///
  /// in MyControl-impl.cpp
  ///
  /// DALI_TYPE_REGISTRATION_BEGIN( Toolkit::MyControl, Toolkit::Control, Create );
  /// DALI_PROPERTY_REGISTRATION( Toolkit, MyControl, "Hours",  INTEGER, DISABLED                     )
  /// DALI_TYPE_REGISTRATION_END()
  /// </remarks>
  ///
  ///
  internal class ScriptableProperty : System.Attribute
  {
    public enum ScriptableType
    {
      Default,    // Read Writable, non-animatable property, event thread only
    //  Animatable // Animatable property, Currently disabled, UK
    }
    public readonly ScriptableType type;

    public ScriptableProperty(ScriptableType type = ScriptableType.Default )
    {
      this.type = type;
    }
  }

  /// <summary>
  /// View Registry singleton.
  /// Used for registering controls and any scriptable properties they have ( see ScriptableProperty )
  ///
  /// Internal Design from C# to C++
  ///
  /// - Each custom C# view should have it's static constructor called before any JSON file is loaded.
  /// Static constructors for a class will only run once ( they are run per control type, not per instance).
  /// Example of running a static constructor:
  ///      System.Runtime.CompilerServices.RuntimeHelpers.RunClassConstructor (typeof(Spin).TypeHandle);
  /// Inside the static constructor the control should register it's type with the ViewRegistry
  /// e.g.
  ///
  ///  static Spin()
  ///  {
  ///     ViewRegistry.Instance.Register(CreateInstance, typeof(Spin) );
  ///  }
  ///
  ///  The control should also provide a CreateInstance function, which gets passed to the ViewRegistry
  ///  // Eventually it will be called if DALi Builderfinds a Spin control in a JSON file
  ///  static CustomView CreateInstance()
  ///  {
  ///    return new Spin();
  ///  }
  ///
  ///
  ///
  /// The DALi C++ equivalent of this is
  ///
  ///  TypeRegistration mType( typeid(Toolkit::Spin), typeid(Toolkit::Control), CreateInstance );
  ///
  ///
  ///
  /// </summary>
  internal sealed class ViewRegistry
  {
    /// <summary>
    /// ViewRegistry is a singleton
    /// </summary>
    private static ViewRegistry instance = null;

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate IntPtr CreateControlDelegate( IntPtr cPtrControlName );

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate IntPtr GetPropertyDelegate( IntPtr controlPtr, IntPtr propertyName );

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate void SetPropertyDelegate( IntPtr controlPtr, IntPtr propertyName, IntPtr propertyValue );

    private CreateControlDelegate _createCallback;
    private SetPropertyDelegate _setPropertyCallback;
    private GetPropertyDelegate _getPropertyCallback;
    private PropertyRangeManager _propertyRangeManager;

    /// <summary>
    /// Given a C++ control the dictionary allows us to find which C# control (View) it belongs to.
    /// By keeping the weak reference only, it will allow the object to be garbage collected.
    /// </summary>
    private Dictionary<IntPtr, WeakReference> _controlMap;

    ///<summary>
    // Maps the name of a custom view to a create instance function
    /// E.g. given a string "Spin", we can get a function used to create the Spin View.
    ///</summary>
    private Dictionary<String, Func< CustomView > > _constructorMap;

    /// <summary>
    /// Lookup table to match C# types to DALi types, used for the automatic property registration
    /// </summary>
    private static readonly Dictionary<string, Dali.Property.Type> _daliPropertyTypeLookup
    = new Dictionary< string, Dali.Property.Type  >
    {
      { "float",   Property.Type.FLOAT },
      { "int",     Property.Type.INTEGER },
      { "Int32",   Property.Type.INTEGER },
      { "Boolean", Property.Type.BOOLEAN },
      { "string",  Property.Type.STRING },
      { "Vector2", Property.Type.VECTOR2 },
      { "Vector3", Property.Type.VECTOR3 },
      { "Vector4", Property.Type.VECTOR4 },
      { "Size",    Property.Type.VECTOR2 },
      { "Position",Property.Type.VECTOR3 },
      { "Color",   Property.Type.VECTOR4 },
    //  { "Matrix3", Property.Type.MATRIX3 }, commented out until we need to use Matrices from JSON
    //  { "Matrix",  Property.Type.MATRIX },
    };


    public ViewRegistry()
    {
      _createCallback = new CreateControlDelegate( CreateControl );
      _getPropertyCallback = new GetPropertyDelegate (GetProperty);
      _setPropertyCallback  = new SetPropertyDelegate (SetProperty);

      _controlMap = new Dictionary<IntPtr, WeakReference>();
      _constructorMap = new Dictionary<string, Func<CustomView>>();
      _propertyRangeManager = new PropertyRangeManager();

    }

    private Dali.Property.Type GetDaliPropertyType( string cSharpTypeName )
    {
      Dali.Property.Type daliType;
      if ( _daliPropertyTypeLookup.TryGetValue (cSharpTypeName, out daliType) )
      {
        //Console.WriteLine("mapped "+ cSharpTypeName + " to dAli type " +daliType );
        return daliType;
      }
      else
      {
       // Console.WriteLine("Failed to find a mapping between C# property" + cSharpTypeName +" and DALi type");
        return Property.Type.NONE;
      }
    }

    /// <summary>
    /// Called directly from DALi C++ type registry to create a control (View) using no marshalling.
    /// </summary>
    /// <returns>Pointer to the Control (Views) handle </returns>
    /// <param name="cPtrControlName"> C pointer to the Control (View) name</param>
    private static IntPtr CreateControl( IntPtr cPtrControlName )
    {
      string controlName = System.Runtime.InteropServices.Marshal.PtrToStringAnsi (cPtrControlName);
      // Console.WriteLine ("Create controlled called from C++ create a " + controlName);

      Func< CustomView > controlConstructor;

      // find the control constructor
      if ( Instance._constructorMap.TryGetValue (controlName, out controlConstructor) )
      {
        // Create the control
        CustomView newControl = controlConstructor ();
        return newControl.GetPtrfromView();  // return pointer to handle
      }
      else
      {
        throw new global::System.InvalidOperationException("C# View not registererd with ViewRegistry"+ controlName );
        return IntPtr.Zero;
      }
    }

    /// <summary>
    /// Store the mapping between this instance of control (View) and native part.
    /// </summary>
    /// <param name="view"> The instance of control (View)</param>
    public static void RegisterView( View view )
    {
      // We store a pointer to the RefObject for the control
      RefObject refObj = view.GetObjectPtr();
      IntPtr refCptr = (IntPtr) RefObject.getCPtr(refObj);

      //Console.WriteLine ("________Storing ref object cptr in control map Hex: {0:X}", refCptr);
      if ( !Instance._controlMap.ContainsKey(refCptr) )
      {
        Instance._controlMap.Add(refCptr, new WeakReference(view, false));
      }

      return;
    }

    /// <summary>
    /// Remove the this instance of control (View) and native part from the mapping table.
    /// </summary>
    /// <param name="view"> The instance of control (View)</param>
    public static void UnregisterView( View view )
    {
      RefObject refObj = view.GetObjectPtr();
      IntPtr refCptr = (IntPtr) RefObject.getCPtr(refObj);

      if ( Instance._controlMap.ContainsKey(refCptr) )
      {
        Instance._controlMap.Remove(refCptr);
      }

      return;
    }

    private static IntPtr GetProperty( IntPtr controlPtr, IntPtr propertyName )
    {
      string name = System.Runtime.InteropServices.Marshal.PtrToStringAnsi (propertyName);
      return Instance.GetPropertyValue ( controlPtr, name);
    }

    private static void SetProperty(  IntPtr controlPtr, IntPtr propertyName, IntPtr propertyValue )
    {
      string name = System.Runtime.InteropServices.Marshal.PtrToStringAnsi (propertyName);
      //Console.WriteLine ( SetControlProperty  called for:" + name );
      Instance.SetPropertyValue ( controlPtr, name, propertyValue);

    }

    public static ViewRegistry Instance
    {
      get
      {
        if (instance==null)
        {
          instance = new ViewRegistry();
        }
        return instance;
      }
    }

    public static View GetViewFromActor( View view )
    {
      // we store a dictionary of ref-obects (C++ land) to custom views (C# land)

      RefObject refObj = view.GetObjectPtr ();
      IntPtr refObjectPtr = (IntPtr) RefObject.getCPtr(refObj);

      WeakReference viewReference;
      if ( Instance._controlMap.TryGetValue ( refObjectPtr, out viewReference) )
      {
        View retview = viewReference.Target as View;
        return retview;
      }
      else
      {
        return null;
      }
    }


    /// <summary>
    /// Function which registers a view and all it's scriptable properties with DALi's type registry.
    /// Means the View can be created / configured from a JSON script.
    ///
    /// The function uses introspection to scan a Views C# properties, then selects the ones with
    ///[ScriptableProperty] attribute to be registered.
    /// Example of a Spin view registering itself
    ///   static Spin()
    /// {
    ///   ViewRegistry registers control type with DALi type registery
    ///   also uses introspection to find any properties that need to be registered with type registry
    ///   ViewRegistry.Instance.Register(CreateInstance, typeof(Spin) );
    /// }
    ///
    /// </summary>
    public void Register(Func< CustomView > createFunction, System.Type viewType )
    {
      // add the mapping between the view name and it's create function
      _constructorMap.Add (viewType.Name, createFunction);

      // Call into DALi C++ to register the control with the type registry
      TypeRegistration.RegisterControl( viewType.Name, _createCallback );

      // Cycle through each property in the class
      foreach (System.Reflection.PropertyInfo propertyInfo in viewType.GetProperties())
      {

        if ( propertyInfo.CanRead )
        {

          System.Attribute[] attrs = System.Attribute.GetCustomAttributes(propertyInfo);
          foreach (System.Attribute attr in attrs)
          {
            // If the Scriptable attribute exists, then register it with the type registry.
            if (attr is ScriptableProperty)
            {
              //Console.WriteLine ("Got a DALi JSON scriptable property = " + propertyInfo.Name +", of type " + propertyInfo.PropertyType.Name);

              // first get the attribute type, ( default, or animatable)
              ScriptableProperty scriptableProp = attr as ScriptableProperty;

              // we get the start property index, based on the type and it's heirachy, e.g. DateView (70,000)-> Spin (60,000) -> View (50,000)
              int propertyIndex =  _propertyRangeManager.GetPropertyIndex( viewType.Name, viewType, scriptableProp.type );

              // get the enum for the property type... E.g. registering a string property returns Dali.PropertyType.String
              Dali.Property.Type propertyType = GetDaliPropertyType( propertyInfo.PropertyType.Name );

              // Example   RegisterProperty("spin","maxValue", 50001, FLOAT, set, get );
              // Native call to register the property
              TypeRegistration.RegisterProperty (viewType.Name, propertyInfo.Name , propertyIndex, propertyType, _setPropertyCallback, _getPropertyCallback);
            }
          }
          // Console.WriteLine ("property name = " + propertyInfo.Name);
        }
      }
    }

    /// <summary>
    /// Get a property value from a View
    ///
    /// </summary>
    private IntPtr GetPropertyValue ( IntPtr controlPtr, string propertyName)
    {
      // Get the C# control that maps to the C++ control
      BaseHandle baseHandle = new BaseHandle (controlPtr, false);

      RefObject refObj = baseHandle.GetObjectPtr ();

      IntPtr refObjectPtr = (IntPtr) RefObject.getCPtr(refObj);

      WeakReference viewReference;
      if ( _controlMap.TryGetValue ( refObjectPtr, out viewReference) )
      {
        View view = viewReference.Target as View;

        // call the get property function
        System.Object val = view.GetType ().GetProperty (propertyName).GetAccessors () [0].Invoke (view, null);

        Property.Value value = Property.Value.CreateFromObject (val);

        return (IntPtr)Property.Value.getCPtr (value);
      }
      else
      {
        return IntPtr.Zero;
      }
    }

    /// <summary>
    /// Set a property value on a View
    ///
    /// </summary>
    private void SetPropertyValue ( IntPtr controlPtr, string propertyName, IntPtr propertyValuePtr)
    {
      // Get the C# control that maps to the C++ control

      //Console.WriteLine ("SetPropertyValue   refObjectPtr = {0:X}", controlPtr);

      Property.Value propValue = new Property.Value (propertyValuePtr, false);

      WeakReference viewReference;
      if ( _controlMap.TryGetValue ( controlPtr, out viewReference) )
      {
        View view = viewReference.Target as View;
        System.Reflection.PropertyInfo propertyInfo = view.GetType().GetProperty(propertyName);

        // We know the property name, we know it's type, we just need to convert from a DALi property value to native C# type
        System.Type type = propertyInfo.PropertyType;
        bool ok = false;

        if ( type.Equals (typeof(Int32)) )
        {
          int value = 0;
          ok = propValue.Get( out value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(bool)) )
        {
          bool value = false;
          ok = propValue.Get( out value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(float)) )
        {
          float value = 0;
          ok = propValue.Get( out value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(string)) )
        {
          string value = "";
          ok = propValue.Get( out value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(Vector2)) )
        {
          Vector2 value = new Vector2 ();
          ok = propValue.Get( value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(Vector3)) )
        {
          Vector3 value = new Vector3 ();
          ok = propValue.Get( value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(Vector4)) )
        {
          Vector4 value = new Vector4 ();
          ok = propValue.Get( value );

          if ( ok )
          {
           propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(Position)) )
        {
          Position value = new Position ();
          ok = propValue.Get( value );
          if ( ok )
          {
            propertyInfo.SetValue (view, value);
          }
        }
        else if ( type.Equals (typeof(Size)) )
        {
          Size value = new Size();
          ok = propValue.Get( value );
          if ( ok )
          {
            propertyInfo.SetValue(view, new Size(value.Width, value.Height, value.Depth));
          };
        }
        else if ( type.Equals (typeof(Color)) )
        {
          // Colors are stored as Vector4's in DALi
          Vector4 value = new Vector4();
          ok = propValue.Get( value );
          if ( ok )
          {
            propertyInfo.SetValue (view, (Color)value);
          };
        }
        else
        {
          throw new global::System.InvalidOperationException("SetPropertyValue Unimplemented type for Property Value");
        }
        if ( !ok )
        {
          throw new global::System.InvalidOperationException("SetPropertyValue propValue.Get failed");
        }
      }
      else
      {
        throw new global::System.InvalidOperationException("failed to find the control to write a property to: cptr = " + controlPtr);
      }

    }

  }


}