//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (http://www.swig.org).
// Version 3.0.10
//
// Do not make changes to this file unless you know what you are doing--modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------

namespace Dali {

public class LongPressGesture : Gesture {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal LongPressGesture(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NDalicPINVOKE.LongPressGesture_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(LongPressGesture obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~LongPressGesture() {
    DisposeQueue.Instance.Add(this);
  }

  public override void Dispose() {
    if (!Stage.IsInstalled()) {
      DisposeQueue.Instance.Add(this);
      return;
    }

    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicPINVOKE.delete_LongPressGesture(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }


  public static LongPressGesture GetLongPressGestureFromPtr(global::System.IntPtr cPtr) {
    LongPressGesture ret = new LongPressGesture(cPtr, false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint NumberOfTouches
  {
    get
    {
      return numberOfTouches;
    }
  }

  public Vector2 ScreenPoint
  {
    get
    {
      return screenPoint;
    }
  }

  public Vector2 LocalPoint
  {
    get
    {
      return localPoint;
    }
  }

  public LongPressGesture(Gesture.StateType state) : this(NDalicPINVOKE.new_LongPressGesture__SWIG_0((int)state), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public LongPressGesture(LongPressGesture rhs) : this(NDalicPINVOKE.new_LongPressGesture__SWIG_1(LongPressGesture.getCPtr(rhs)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public LongPressGesture Assign(LongPressGesture rhs) {
    LongPressGesture ret = new LongPressGesture(NDalicPINVOKE.LongPressGesture_Assign(swigCPtr, LongPressGesture.getCPtr(rhs)), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private uint numberOfTouches {
    set {
      NDalicPINVOKE.LongPressGesture_numberOfTouches_set(swigCPtr, value);
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      uint ret = NDalicPINVOKE.LongPressGesture_numberOfTouches_get(swigCPtr);
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  private Vector2 screenPoint {
    set {
      NDalicPINVOKE.LongPressGesture_screenPoint_set(swigCPtr, Vector2.getCPtr(value));
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      global::System.IntPtr cPtr = NDalicPINVOKE.LongPressGesture_screenPoint_get(swigCPtr);
      Vector2 ret = (cPtr == global::System.IntPtr.Zero) ? null : new Vector2(cPtr, false);
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  private Vector2 localPoint {
    set {
      NDalicPINVOKE.LongPressGesture_localPoint_set(swigCPtr, Vector2.getCPtr(value));
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      global::System.IntPtr cPtr = NDalicPINVOKE.LongPressGesture_localPoint_get(swigCPtr);
      Vector2 ret = (cPtr == global::System.IntPtr.Zero) ? null : new Vector2(cPtr, false);
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

}

}
