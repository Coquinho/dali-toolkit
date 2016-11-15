/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

using System;
using System.Runtime.InteropServices;
using Dali;

namespace MyCSharpExample
{
  class Example
  {
    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate void CallbackDelegate(IntPtr appPtr); // void, void delgate

    private Dali.Application _application;

    public Example(Dali.Application application)
    {
      _application = application;
      Console.WriteLine( "InitSignal connection count = " + _application.InitSignal().GetConnectionCount() );

      _application.Initialized += Initialize;
      Console.WriteLine( "InitSignal connection count = " + _application.InitSignal().GetConnectionCount() );
    }

    public void Initialize(object source, AUIApplicationInitEventArgs e)
    {

      OperatorTests();

      Handle handle = new Handle();
      int myPropertyIndex = handle.RegisterProperty("myProperty", new Property.Value(10.0f), Property.AccessMode.READ_WRITE);
      float myProperty = 0.0f;
      handle.GetProperty(myPropertyIndex).Get(ref myProperty);
      Console.WriteLine( "myProperty value: " + myProperty );

      int myPropertyIndex2 = handle.RegisterProperty("myProperty2", new Property.Value(new Size(5.0f, 5.0f)), Property.AccessMode.READ_WRITE);
      Size myProperty2 = new Size(0.0f, 0.0f);
      handle.GetProperty(myPropertyIndex2).Get(myProperty2);
      Console.WriteLine( "myProperty2 value: " + myProperty2.x + ", " + myProperty2.y );

      Actor actor = new Actor();
      actor.Size = new Position(200.0f, 200.0f, 0.0f);
      actor.Name = "MyActor";
      actor.Color = new Color(1.0f, 0.0f, 1.0f, 0.8f);
      Console.WriteLine("Actor id: {0}", actor.GetId());
      Console.WriteLine("Actor size: " + actor.Size.x + ", " + actor.Size.y);
      Console.WriteLine("Actor name: " + actor.Name);

      Stage stage = Stage.GetCurrent();
      stage.BackgroundColor =  new Color("white") ;

      Size stageSize = stage.Size;
      Console.WriteLine("Stage size: " + stageSize.x + ", " + stageSize.y);
      stage.Add(actor);

      TextLabel text = new TextLabel("Hello Mono World");
      text.ParentOrigin = NDalic.ParentOriginCenter;
      text.AnchorPoint = NDalic.AnchorPointCenter;
      text.HorizontalAlignment = "CENTER";
      stage.Add(text);

      Console.WriteLine( "Text label text:  " + text.Text );

      Console.WriteLine( "Text label point size:  " + text.PointSize );
      text.PointSize = 32.0f;
      Console.WriteLine( "Text label new point size:  " + text.PointSize );

      using (RectInteger ri = new RectInteger(02,05,20,30))
      {
        Console.WriteLine( "    Created " + ri );
        Console.WriteLine( "    IsEmpty() =  " + ri.IsEmpty() );
        Console.WriteLine( "    Left =  " + ri.Left() );
        Console.WriteLine( "    Right =  " + ri.Right() );
        Console.WriteLine( "    Top  = " + ri.Top() );
        Console.WriteLine( "    Bottom  = " + ri.Bottom() );
        Console.WriteLine( "    Area  = " + ri.Area() );
      }
      Console.WriteLine( " *************************" );
      using (RectInteger ri2 = new RectInteger(02,05,20,30))
      {
        Console.WriteLine( "    Created " + ri2 );
        ri2.Set(1,1,40,40);
        Console.WriteLine( "    IsEmpty() =  " + ri2.IsEmpty() );
        Console.WriteLine( "    Left =  " + ri2.Left() );
        Console.WriteLine( "    Right =  " + ri2.Right() );
        Console.WriteLine( "    Top  = " + ri2.Top() );
        Console.WriteLine( "    Bottom  = " + ri2.Bottom() );
        Console.WriteLine( "    Area  = " + ri2.Area() );
      }
      Console.WriteLine( " *************************" );
      using (RectDouble rd = new RectDouble(02,05,20.5,30.5))
      {
        Console.WriteLine( "    Created " + rd );
        Console.WriteLine( "    IsEmpty() =  " + rd.IsEmpty() );
        Console.WriteLine( "    Left =  " + rd.Left() );
        Console.WriteLine( "    Right =  " + rd.Right() );
        Console.WriteLine( "    Top  = " + rd.Top() );
        Console.WriteLine( "    Bottom  = " + rd.Bottom() );
        Console.WriteLine( "    Area  = " + rd.Area() );
      }
      Console.WriteLine( " *************************" );
      RectDouble rd2 = new RectDouble();
      rd2.x = 10;
      rd2.y = 10;
      rd2.width = 20;
      rd2.height = 20;
      Console.WriteLine( "    Created " + rd2 );
      Console.WriteLine( "    IsEmpty() =  " + rd2.IsEmpty() );
      Console.WriteLine( "    Left =  " + rd2.Left() );
      Console.WriteLine( "    Right =  " + rd2.Right() );
      Console.WriteLine( "    Top  = " + rd2.Top() );
      Console.WriteLine( "    Bottom  = " + rd2.Bottom() );
      Console.WriteLine( "    Area  = " + rd2.Area() );

      Console.WriteLine( " *************************" );
      Size Size = new Size(100, 50);
      Console.WriteLine( "    Created " + Size );
      Console.WriteLine( "    Size x =  " + Size.x + ", y = " + Size.y );
      Size += new Size(20, 20);
      Console.WriteLine( "    Size x =  " + Size[0] + ", y = " + Size[1] );
      Size.x += 10;
      Size.y += 10;
      Console.WriteLine( "    Size width =  " + Size.width + ", height = " + Size.height );
      Size += new Size(15, 15);
      Console.WriteLine( "    Size width =  " + Size[0] + ", height = " + Size[1] );

      Console.WriteLine( " *************************" );
      Position Position = new Position(20, 100, 50);
      Console.WriteLine( "    Created " + Position );
      Console.WriteLine( "    Position x =  " + Position.x + ", y = " + Position.y + ", z = " + Position.z );
      Position += new Position(20, 20, 20);
      Console.WriteLine( "    Position x =  " + Position[0] + ", y = " + Position[1] + ", z = " + Position[2] );
      Position.x += 10;
      Position.y += 10;
      Position.z += 10;
      Console.WriteLine( "    Position width =  " + Position.width + ", height = " + Position.height + ", depth = " + Position.depth );
      Position parentOrigin = new Dali.Position(NDalic.ParentOriginBottomRight);
      Console.WriteLine( "    parentOrigin x =  " + parentOrigin.x + ", y = " + parentOrigin.y + ", z = " + parentOrigin.z );

      Console.WriteLine( " *************************" );
      Color Color = new Color(20, 100, 50, 200);
      Console.WriteLine( "    Created " + Color );
      Console.WriteLine( "    Color x =  " + Color.x + ", y = " + Color.y + ", z = " + Color.z + ", w = " + Color.w );
      Color += new Color(20, 20, 20, 20);
      Console.WriteLine( "    Color x =  " + Color[0] + ", y = " + Color[1] + ", z = " + Color[2] + ", w = " + Color[3] );
      Color.x += 10;
      Color.y += 10;
      Color.z += 10;
      Color.w += 10;
      Console.WriteLine( "    Color r =  " + Color.r + ", g = " + Color.g + ", b = " + Color.b + ", a = " + Color.a );
    }


  public void OperatorTests()
  {
    Actor actor = new Actor();
    Actor differentActor = new Actor();
    Actor actorSame = actor;
    Actor nullActor = null;

      // test the true operator
    if ( actor )
    {
      Console.WriteLine ("BaseHandle Operator true (actor) : test passed ");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator true (actor): test failed ");
    }

    Actor parent = actor.GetParent ();

    if ( parent )
    {
      Console.WriteLine ("Handle with Empty body  :failed ");
    }
    else
    {
      Console.WriteLine ("Valid with Empty body  :passed ");
    }

    actor.Add( differentActor );
    // here we test two different C# objects, which on the native side have the same body/ ref-object
    if ( actor == differentActor.GetParent() )
    {
       Console.WriteLine ("actor == differentActor.GetParent() :passed ");
    }
    else
    {
      Console.WriteLine ("actor == differentActor.GetParent() :failed ");
    }

    if ( differentActor == differentActor.GetParent() )
    {
       Console.WriteLine ("differentActor == differentActor.GetParent() :failed ");
    }
    else
    {
      Console.WriteLine ("differentActor == differentActor.GetParent() :passed ");
    }


    if ( nullActor )
    {
      Console.WriteLine ("BaseHandle Operator true (nullActor) : test failed ");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator true (nullActor): test passed ");
    }

    // ! operator
    if ( !actor )
    {
      Console.WriteLine ("BaseHandle Operator !(actor) : test failed ");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator !(actor): test passed ");
    }

    if ( !nullActor )
    {
      Console.WriteLine ("BaseHandle Operator !(nullActor) : test passed ");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator !(nullActor): test failed ");
    }

    // Note: operator false only used inside & operator
    // test equality operator ==
    if ( actor == actorSame )
    {
      Console.WriteLine ("BaseHandle Operator  (actor == actorSame) : test passed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator  (actor == actorSame) : test failed");
    }

    if ( actor == differentActor )
    {
      Console.WriteLine ("BaseHandle Operator (actor == differentActor) : test failed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (actor == differentActor) : test passed");
    }

    if ( actor == nullActor )
    {
      Console.WriteLine ("BaseHandle Operator (actor == nullActor) : test failed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (actor == nullActor) : test passed");
    }

    if ( nullActor == nullActor )
    {
      Console.WriteLine ("BaseHandle Operator (nullActor == nullActor) : test passed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (nullActor == nullActor) : test failed");
    }

    // test || operator
    if ( actor || actorSame )
    {
      Console.WriteLine ("BaseHandle Operator (actor || actorSame) : test passed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (actor || actorSame) : test failed");
    }

    if ( actor || nullActor )
    {
      Console.WriteLine ("BaseHandle Operator (actor || nullActor) : test passed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (actor || nullActor) : test failed");
    }

    if ( nullActor || nullActor )
    {
      Console.WriteLine ("BaseHandle Operator (nullActor || nullActor) : test failed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (nullActor || nullActor) : test passed");
    }


    // test && operator
    if ( actor && actorSame )
    {
      Console.WriteLine ("BaseHandle Operator (actor && actorSame) : test passed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (actor && actorSame) : test failed");
    }

    if ( actor && nullActor )
    {
      Console.WriteLine ("BaseHandle Operator (actor && nullActor) : test failed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (actor && nullActor) : test passed");
    }

    if ( nullActor && nullActor )
    {
      Console.WriteLine ("BaseHandle Operator (nullActor && nullActor) : test failed");
    }
    else
    {
      Console.WriteLine ("BaseHandle Operator (nullActor && nullActor) : test passed");
    }

  }

    public void MainLoop()
    {
      _application.MainLoop ();
    }

    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    [STAThread]
      static void Main(string[] args)
      {
        Console.WriteLine ("Hello Mono World");

        Example example = new Example(Application.NewApplication());
        example.MainLoop ();
      }
  }
}
