/** Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

namespace Dali {

    using System;
    using System.Runtime.InteropServices;


public class View : CustomActor {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal View(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NDalicPINVOKE.View_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);

    // Register this instance of view in the view registry.
    ViewRegistry.RegisterView(this);

    // By default, we do not want the position to use the anchor point
    this.PositionUsesAnchorPoint = false;
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(View obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~View() {
    DisposeQueue.Instance.Add(this);

    // Unregister this instance of view from the view registry.
    ViewRegistry.UnregisterView(this);
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
          NDalicPINVOKE.delete_View(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }

  /**
   * @brief Event arguments that passed via KeyInputFocusGained signal
   *
   */
  public class KeyInputFocusGainedEventArgs : EventArgs
  {
    private View _view;

    /**
     * @brief View - is the view that gets Key Input Focus
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via KeyInputFocusLost signal
   *
   */
  public class KeyInputFocusLostEventArgs : EventArgs
  {
    private View _view;

    /**
     * @brief View - is the view that loses Key Input Focus
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via Key signal
   *
   */
  public class KeyEventArgs : EventArgs
  {
    private View _view;
    private Key _key;

    /**
     * @brief View - is the view that recieves the key.
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }

    /**
     * @brief Key - is the key sent to the View.
     *
     */
    public Key Key
    {
      get
      {
        return _key;
      }
      set
      {
        _key = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via OnRelayout signal
   *
   */
  public class OnRelayoutEventArgs : EventArgs
  {
    private View _view;

    /**
     * @brief View - is the view that is being resized upon relayout
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }
  }


  /**
   * @brief Event arguments that passed via Touch signal
   *
   */
  public class TouchEventArgs : EventArgs
  {
     private View _view;
     private Touch _touch;

    /**
     * @brief View - is the view that is being touched
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }

    /**
     * @brief Touch - contains the information of touch points
     *
     */
    public Touch Touch
    {
      get
      {
        return _touch;
      }
      set
      {
        _touch = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via Hover signal
   *
   */
  public class HoverEventArgs : EventArgs
  {
     private View _view;
     private Hover _hover;

    /**
     * @brief View - is the view that is being hovered
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }

    /**
     * @brief Hover - contains touch points that represent the points
     * that are currently being hovered or the points where a hover has stopped
     *
     */
    public Hover Hover
    {
      get
      {
        return _hover;
      }
      set
      {
        _hover = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via Wheel signal
   *
   */
  public class WheelEventArgs : EventArgs
  {
    private View _view;
    private Wheel _wheel;

    /**
     * @brief View - is the view that is being wheeled
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }

    /**
     * @brief Wheel - store a wheel rolling type : MOUSE_WHEEL or CUSTOM_WHEEL
     *
     */
    public Wheel Wheel
    {
      get
      {
        return _wheel;
      }
      set
      {
        _wheel = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via OnStage signal
   *
   */
  public class OnStageEventArgs : EventArgs
  {
  private View _view;

    /**
     * @brief View - is the view that is being connected to the stage
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }
  }

  /**
   * @brief Event arguments that passed via OffStage signal
   *
   */
  public class OffStageEventArgs : EventArgs
  {
  private View _view;

    /**
     * @brief View - is the view that is being disconnected from the stage
     *
     */
    public View View
    {
      get
      {
        return _view;
      }
      set
      {
        _view = value;
      }
    }
  }

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate void KeyInputFocusGainedCallbackDelegate(IntPtr control);
  private DaliEventHandler<object,KeyInputFocusGainedEventArgs> _KeyInputFocusGainedEventHandler;
  private KeyInputFocusGainedCallbackDelegate _KeyInputFocusGainedCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate void KeyInputFocusLostCallbackDelegate(IntPtr control);
  private DaliEventHandler<object,KeyInputFocusLostEventArgs> _KeyInputFocusLostEventHandler;
  private KeyInputFocusLostCallbackDelegate _KeyInputFocusLostCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate bool KeyCallbackDelegate(IntPtr control, IntPtr key);
  private DaliEventHandlerWithReturnType<object,KeyEventArgs,bool> _KeyHandler;
  private KeyCallbackDelegate _KeyCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate void OnRelayoutEventCallbackDelegate(IntPtr control);
  private DaliEventHandler<object,OnRelayoutEventArgs> _viewOnRelayoutEventHandler;
  private OnRelayoutEventCallbackDelegate _viewOnRelayoutEventCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate bool TouchCallbackDelegate(IntPtr view, IntPtr touch);
  private DaliEventHandlerWithReturnType<object,TouchEventArgs,bool> _viewTouchHandler;
  private TouchCallbackDelegate _viewTouchCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate bool HoverCallbackDelegate(IntPtr view, IntPtr hover);
  private DaliEventHandlerWithReturnType<object,HoverEventArgs,bool> _viewHoverHandler;
  private HoverCallbackDelegate _viewHoverCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate bool WheelCallbackDelegate(IntPtr view, IntPtr wheel);
  private DaliEventHandlerWithReturnType<object,WheelEventArgs,bool> _viewWheelHandler;
  private WheelCallbackDelegate _viewWheelCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate void OnStageEventCallbackDelegate(IntPtr control);
  private DaliEventHandler<object,OnStageEventArgs> _viewOnStageEventHandler;
  private OnStageEventCallbackDelegate _viewOnStageEventCallbackDelegate;

  [UnmanagedFunctionPointer(CallingConvention.StdCall)]
  private delegate void OffStageEventCallbackDelegate(IntPtr control);
  private DaliEventHandler<object,OffStageEventArgs> _viewOffStageEventHandler;
  private OffStageEventCallbackDelegate _viewOffStageEventCallbackDelegate;

  /**
   * @brief Event for KeyInputFocusGained signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of KeyInputFocusGainedEventHandler-DaliEventHandler<object,KeyInputFocusGainedEventArgs>)
   * provided by the user. KeyInputFocusGained signal is emitted when the control gets Key Input Focus.
   */
  public event DaliEventHandler<object,KeyInputFocusGainedEventArgs> KeyInputFocusGained
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_KeyInputFocusGainedEventHandler == null)
        {
          _KeyInputFocusGainedEventHandler += value;
          Console.WriteLine("View Key EVENT Locked....");
          _KeyInputFocusGainedCallbackDelegate = new KeyInputFocusGainedCallbackDelegate(OnKeyInputFocusGained);
          this.KeyInputFocusGainedSignal().Connect(_KeyInputFocusGainedCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_KeyInputFocusGainedEventHandler != null)
        {
          this.KeyInputFocusGainedSignal().Disconnect(_KeyInputFocusGainedCallbackDelegate);
        }

        _KeyInputFocusGainedEventHandler -= value;
      }
    }
  }

  private void OnKeyInputFocusGained(IntPtr view)
  {
    KeyInputFocusGainedEventArgs e = new KeyInputFocusGainedEventArgs();
    Console.WriteLine("View Key ....");
    // Populate all members of "e" (KeyInputFocusGainedEventArgs) with real data
    e.View = Dali.View.GetViewFromPtr(view);

    if (_KeyInputFocusGainedEventHandler != null)
    {
      //here we send all data to user event handlers
      _KeyInputFocusGainedEventHandler(this, e);
    }

  }

  /**
   * @brief Event for KeyInputFocusLost signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of KeyInputFocusLostEventHandler-DaliEventHandler<object,KeyInputFocusLostEventArgs>)
   * provided by the user. KeyInputFocusLost signal is emitted when the control loses Key Input Focus.
   */
  public event DaliEventHandler<object,KeyInputFocusLostEventArgs> KeyInputFocusLost
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_KeyInputFocusLostEventHandler == null)
        {
          _KeyInputFocusLostEventHandler += value;

          _KeyInputFocusLostCallbackDelegate = new KeyInputFocusLostCallbackDelegate(OnKeyInputFocusLost);
          this.KeyInputFocusLostSignal().Connect(_KeyInputFocusLostCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_KeyInputFocusLostEventHandler != null)
        {
          this.KeyInputFocusLostSignal().Disconnect(_KeyInputFocusLostCallbackDelegate);
        }

        _KeyInputFocusLostEventHandler -= value;
      }
    }
  }

  private void OnKeyInputFocusLost(IntPtr view)
  {
    KeyInputFocusLostEventArgs e = new KeyInputFocusLostEventArgs();

    // Populate all members of "e" (KeyInputFocusLostEventArgs) with real data
    e.View = Dali.View.GetViewFromPtr(view);

    if (_KeyInputFocusLostEventHandler != null)
    {
      //here we send all data to user event handlers
      _KeyInputFocusLostEventHandler(this, e);
    }
  }

  /**
   * @brief Event for KeyPressed signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of KeyHandler-DaliEventHandlerWithReturnType<object,KeyEventArgs,bool>)
   * provided by the user. KeyPressed signal is emitted when key event is received.
   */
  public event DaliEventHandlerWithReturnType<object,KeyEventArgs,bool> KeyPressed
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_KeyHandler == null)
        {
          _KeyHandler += value;

          _KeyCallbackDelegate = new KeyCallbackDelegate(OnKey);
          this.KeyEventSignal().Connect(_KeyCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_KeyHandler != null)
        {
          this.KeyEventSignal().Disconnect(_KeyCallbackDelegate);
        }

        _KeyHandler -= value;
      }
    }
  }

  private bool OnKey(IntPtr view, IntPtr key)
  {
    KeyEventArgs e = new KeyEventArgs();

    // Populate all members of "e" (KeyEventArgs) with real data
    e.View = Dali.View.GetViewFromPtr(view);
    e.Key = Dali.Key.GetKeyFromPtr(key);

    if (_KeyHandler != null)
    {
      //here we send all data to user event handlers
      return _KeyHandler(this, e);
    }
    return false;

  }

  /**
   * @brief Event for OnRelayout signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of OnRelayoutEventHandler) provided by the user.
   * OnRelayout signal is emitted after the size has been set on the view during relayout.
   */
  public event DaliEventHandler<object,OnRelayoutEventArgs> OnRelayoutEvent
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_viewOnRelayoutEventHandler == null)
        {
          _viewOnRelayoutEventHandler += value;
          Console.WriteLine("View OnRelayoutEventArgs Locked....");
          _viewOnRelayoutEventCallbackDelegate = new OnRelayoutEventCallbackDelegate(OnRelayout);
          this.OnRelayoutSignal().Connect(_viewOnRelayoutEventCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_viewOnRelayoutEventHandler != null)
        {
          this.OnRelayoutSignal().Disconnect(_viewOnRelayoutEventCallbackDelegate);
        }

        _viewOnRelayoutEventHandler -= value;
      }
    }
  }

  // Callback for View OnRelayout signal
  private void OnRelayout(IntPtr data)
  {
    OnRelayoutEventArgs e = new OnRelayoutEventArgs();
    Console.WriteLine("View OnRelayoutEventArgs....");
    // Populate all members of "e" (OnRelayoutEventArgs) with real data
    e.View = View.GetViewFromPtr(data);

    if (_viewOnRelayoutEventHandler != null)
    {
      //here we send all data to user event handlers
      _viewOnRelayoutEventHandler(this, e);
    }
  }

  /**
   * @brief Event for Touched signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of TouchHandler-DaliEventHandlerWithReturnType<object,TouchEventArgs,bool>)
   * provided by the user. Touched signal is emitted when touch input is received.
   */
  public event DaliEventHandlerWithReturnType<object,TouchEventArgs,bool> Touched
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_viewTouchHandler == null)
        {
          _viewTouchHandler += value;
          Console.WriteLine("View Touch EVENT LOCKED....");
          _viewTouchCallbackDelegate = new TouchCallbackDelegate(OnTouch);
          this.TouchSignal().Connect(_viewTouchCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_viewTouchHandler != null)
        {
          this.TouchSignal().Disconnect(_viewTouchCallbackDelegate);
        }

        _viewTouchHandler -= value;
      }
    }
  }

  // Callback for View TouchSignal
  private bool OnTouch(IntPtr view, IntPtr touch)
  {
    TouchEventArgs e = new TouchEventArgs();
    Console.WriteLine("View Touch EVENT....");
    // Populate all members of "e" (TouchEventArgs) with real data
    e.View = View.GetViewFromPtr(view);
    e.Touch = Dali.Touch.GetTouchFromPtr(touch);

    if (_viewTouchHandler != null)
    {
      //here we send all data to user event handlers
      return _viewTouchHandler(this, e);
    }

    return false;
  }

  /**
   * @brief Event for Hovered signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of HoverHandler-DaliEventHandlerWithReturnType<object,HoverEventArgs,bool>)
   * provided by the user. Hovered signal is emitted when hover input is received.
   */
  public event DaliEventHandlerWithReturnType<object,HoverEventArgs,bool> Hovered
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_viewHoverHandler == null)
        {
          _viewHoverHandler += value;

          _viewHoverCallbackDelegate = new HoverCallbackDelegate(OnHover);
          this.HoveredSignal().Connect(_viewHoverCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_viewHoverHandler != null)
        {
          this.HoveredSignal().Disconnect(_viewHoverCallbackDelegate);
        }

        _viewHoverHandler -= value;
      }
    }
  }

  // Callback for View Hover signal
  private bool OnHover(IntPtr view, IntPtr hover)
  {
    HoverEventArgs e = new HoverEventArgs();

    // Populate all members of "e" (HoverEventArgs) with real data
    e.View = View.GetViewFromPtr(view);
    e.Hover = Dali.Hover.GetHoverFromPtr(hover);

    if (_viewHoverHandler != null)
    {
      //here we send all data to user event handlers
      return _viewHoverHandler(this, e);
    }

    return false;
  }

  /**
   * @brief Event for WheelMoved signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of WheelHandler-DaliEventHandlerWithReturnType<object,WheelEventArgs,bool>)
   * provided by the user. WheelMoved signal is emitted when wheel event is received.
   */
  public event DaliEventHandlerWithReturnType<object,WheelEventArgs,bool> WheelMoved
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_viewWheelHandler == null)
        {
          _viewWheelHandler += value;
          Console.WriteLine("View Wheel EVENT LOCKED....");
          _viewWheelCallbackDelegate = new WheelCallbackDelegate(OnWheel);
          this.WheelEventSignal().Connect(_viewWheelCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_viewWheelHandler != null)
        {
          this.WheelEventSignal().Disconnect(_viewWheelCallbackDelegate);
        }

        _viewWheelHandler -= value;
      }
    }
  }

  // Callback for View Wheel signal
  private bool OnWheel(IntPtr view, IntPtr wheel)
  {
    WheelEventArgs e = new WheelEventArgs();
    Console.WriteLine("View Wheel EVENT ....");
    // Populate all members of "e" (WheelEventArgs) with real data
    e.View = View.GetViewFromPtr(view);
    e.Wheel = Dali.Wheel.GetWheelFromPtr(wheel);

    if (_viewWheelHandler != null)
    {
      //here we send all data to user event handlers
      return _viewWheelHandler(this, e);
    }

    return false;
  }

  /**
   * @brief Event for OnStage signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of OnStageEventHandler) provided by the user.
   * OnStage signal is emitted after the view has been connected to the stage.
   */
  public event DaliEventHandler<object,OnStageEventArgs> OnStageEvent
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_viewOnStageEventHandler == null)
        {
          _viewOnStageEventHandler += value;

          _viewOnStageEventCallbackDelegate = new OnStageEventCallbackDelegate(OnStage);
          this.OnStageSignal().Connect(_viewOnStageEventCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_viewOnStageEventHandler != null)
        {
          this.OnStageSignal().Disconnect(_viewOnStageEventCallbackDelegate);
        }

        _viewOnStageEventHandler -= value;
      }
    }
  }

  // Callback for View OnStage signal
  private void OnStage(IntPtr data)
  {
    OnStageEventArgs e = new OnStageEventArgs();

    // Populate all members of "e" (OnStageEventArgs) with real data
    e.View = View.GetViewFromPtr(data);

    //Console.WriteLine("############# OnStage()! e.View.Name=" + e.View.Name);

    if (_viewOnStageEventHandler != null)
    {
      //here we send all data to user event handlers
      _viewOnStageEventHandler(this, e);
    }
  }

  /**
   * @brief Event for OffStage signal which can be used to subscribe/unsubscribe the event handler
   * (in the type of OffStageEventHandler) provided by the user.
   * OffStage signal is emitted after the view has been disconnected from the stage.
   */
  public event DaliEventHandler<object,OffStageEventArgs> OffStageEvent
  {
    add
    {
      lock(this)
      {
        // Restricted to only one listener
        if (_viewOffStageEventHandler == null)
        {
          _viewOffStageEventHandler += value;

          _viewOffStageEventCallbackDelegate = new OffStageEventCallbackDelegate(OffStage);
          this.OnStageSignal().Connect(_viewOffStageEventCallbackDelegate);
        }
      }
    }

    remove
    {
      lock(this)
      {
        if (_viewOffStageEventHandler != null)
        {
          this.OnStageSignal().Disconnect(_viewOffStageEventCallbackDelegate);
        }

        _viewOffStageEventHandler -= value;
      }
    }
  }

  // Callback for View OffStage signal
  private void OffStage(IntPtr data)
  {
    OffStageEventArgs e = new OffStageEventArgs();

    // Populate all members of "e" (OffStageEventArgs) with real data
    e.View = View.GetViewFromPtr(data);

    if (_viewOffStageEventHandler != null)
    {
      //here we send all data to user event handlers
      _viewOffStageEventHandler(this, e);
    }
  }

  public static View GetViewFromPtr(global::System.IntPtr cPtr) {
    View ret = new View(cPtr, false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public class Property : global::System.IDisposable {
    private global::System.Runtime.InteropServices.HandleRef swigCPtr;
    protected bool swigCMemOwn;

    internal Property(global::System.IntPtr cPtr, bool cMemoryOwn) {
      swigCMemOwn = cMemoryOwn;
      swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    }

    internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Property obj) {
      return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
    }

    ~Property() {
      Dispose();
    }

    public virtual void Dispose() {
      lock(this) {
        if (swigCPtr.Handle != global::System.IntPtr.Zero) {
          if (swigCMemOwn) {
            swigCMemOwn = false;
            NDalicPINVOKE.delete_View_Property(swigCPtr);
          }
          swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
        }
        global::System.GC.SuppressFinalize(this);
      }
    }

    public static readonly int TOOLTIP = NDalicManualPINVOKE.View_Property_TOOLTIP_get();
    public static readonly int STATE = NDalicManualPINVOKE.View_Property_STATE_get();
    public static readonly int SUB_STATE = NDalicManualPINVOKE.View_Property_SUB_STATE_get();
    public static readonly int LEFT_FOCUSABLE_ACTOR_ID = NDalicManualPINVOKE.View_Property_LEFT_FOCUSABLE_ACTOR_ID_get();
    public static readonly int RIGHT_FOCUSABLE_ACTOR_ID = NDalicManualPINVOKE.View_Property_RIGHT_FOCUSABLE_ACTOR_ID_get();
    public static readonly int UP_FOCUSABLE_ACTOR_ID = NDalicManualPINVOKE.View_Property_UP_FOCUSABLE_ACTOR_ID_get();
    public static readonly int DOWN_FOCUSABLE_ACTOR_ID = NDalicManualPINVOKE.View_Property_DOWN_FOCUSABLE_ACTOR_ID_get();

    public Property() : this(NDalicPINVOKE.new_View_Property(), true) {
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    }

    public static readonly int STYLE_NAME = NDalicPINVOKE.View_Property_STYLE_NAME_get();
    public static readonly int BACKGROUND_COLOR = NDalicPINVOKE.View_Property_BACKGROUND_COLOR_get();
    public static readonly int BACKGROUND_IMAGE = NDalicPINVOKE.View_Property_BACKGROUND_IMAGE_get();
    public static readonly int KEY_INPUT_FOCUS = NDalicPINVOKE.View_Property_KEY_INPUT_FOCUS_get();
    public static readonly int BACKGROUND = NDalicPINVOKE.View_Property_BACKGROUND_get();

  }

  public class KeyboardFocus : global::System.IDisposable {
    private global::System.Runtime.InteropServices.HandleRef swigCPtr;
    protected bool swigCMemOwn;

    internal KeyboardFocus(global::System.IntPtr cPtr, bool cMemoryOwn) {
      swigCMemOwn = cMemoryOwn;
      swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
    }

    internal static global::System.Runtime.InteropServices.HandleRef getCPtr(KeyboardFocus obj) {
      return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
    }

    ~KeyboardFocus() {
      Dispose();
    }

    public virtual void Dispose() {
      lock(this) {
        if (swigCPtr.Handle != global::System.IntPtr.Zero) {
          if (swigCMemOwn) {
            swigCMemOwn = false;
            NDalicPINVOKE.delete_View_KeyboardFocus(swigCPtr);
          }
          swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
        }
        global::System.GC.SuppressFinalize(this);
      }
    }

    public KeyboardFocus() : this(NDalicPINVOKE.new_View_KeyboardFocus(), true) {
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    }

    public enum Direction {
      LEFT,
      RIGHT,
      UP,
      DOWN,
      PAGE_UP,
      PAGE_DOWN
    }
  }

  public View () : this (NDalicPINVOKE.View_New(), true) {
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();

  }
  public View(View uiControl) : this(NDalicPINVOKE.new_View__SWIG_1(View.getCPtr(uiControl)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public View Assign(View handle) {
    View ret = new View(NDalicPINVOKE.View_Assign(swigCPtr, View.getCPtr(handle)), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private new static View DownCast(BaseHandle handle) {
    View ret = new View(NDalicPINVOKE.View_DownCast(BaseHandle.getCPtr(handle)), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public static T DownCast<T>(Actor actor) where T : View
  {
      return (T)( ViewRegistry.GetViewFromActor( actor ) );
  }

  private View ConvertIdToView(uint id)
  {
    Actor actor = null;

    if (Parent)
    {
      actor = Parent.FindChildById(id);
    }

    if (!actor)
    {
      actor = Stage.Instance.GetRootLayer().FindChildById(id);
    }

    return View.DownCast<View>(actor);
  }

  public void SetKeyInputFocus() {
    NDalicPINVOKE.View_SetKeyInputFocus(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool HasKeyInputFocus() {
    bool ret = NDalicPINVOKE.View_HasKeyInputFocus(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void ClearKeyInputFocus() {
    NDalicPINVOKE.View_ClearKeyInputFocus(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public PinchGestureDetector GetPinchGestureDetector() {
    PinchGestureDetector ret = new PinchGestureDetector(NDalicPINVOKE.View_GetPinchGestureDetector(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public PanGestureDetector GetPanGestureDetector() {
    PanGestureDetector ret = new PanGestureDetector(NDalicPINVOKE.View_GetPanGestureDetector(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public TapGestureDetector GetTapGestureDetector() {
    TapGestureDetector ret = new TapGestureDetector(NDalicPINVOKE.View_GetTapGestureDetector(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public LongPressGestureDetector GetLongPressGestureDetector() {
    LongPressGestureDetector ret = new LongPressGestureDetector(NDalicPINVOKE.View_GetLongPressGestureDetector(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetStyleName(string styleName) {
    NDalicPINVOKE.View_SetStyleName(swigCPtr, styleName);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public string GetStyleName() {
    string ret = NDalicPINVOKE.View_GetStyleName(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetBackgroundColor(Vector4 color) {
    NDalicPINVOKE.View_SetBackgroundColor(swigCPtr, Vector4.getCPtr(color));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public Vector4 GetBackgroundColor() {
    Vector4 ret = new Vector4(NDalicPINVOKE.View_GetBackgroundColor(swigCPtr), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void SetBackgroundImage(Image image) {
    NDalicPINVOKE.View_SetBackgroundImage(swigCPtr, Image.getCPtr(image));
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public void ClearBackground() {
    NDalicPINVOKE.View_ClearBackground(swigCPtr);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public ControlKeySignal KeyEventSignal() {
    ControlKeySignal ret = new ControlKeySignal(NDalicPINVOKE.View_KeyEventSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public KeyInputFocusSignal KeyInputFocusGainedSignal() {
    KeyInputFocusSignal ret = new KeyInputFocusSignal(NDalicPINVOKE.View_KeyInputFocusGainedSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public KeyInputFocusSignal KeyInputFocusLostSignal() {
    KeyInputFocusSignal ret = new KeyInputFocusSignal(NDalicPINVOKE.View_KeyInputFocusLostSignal(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public View(ViewImpl implementation) : this(NDalicPINVOKE.new_View__SWIG_2(ViewImpl.getCPtr(implementation)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public enum PropertyRange {
    PROPERTY_START_INDEX = PropertyRanges.PROPERTY_REGISTRATION_START_INDEX,
    CONTROL_PROPERTY_START_INDEX = PROPERTY_START_INDEX,
    CONTROL_PROPERTY_END_INDEX = CONTROL_PROPERTY_START_INDEX+1000
  }

  public string StyleName
  {
    get
    {
      string temp;
      GetProperty( View.Property.STYLE_NAME).Get( out temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.STYLE_NAME, new Dali.Property.Value( value ) );
    }
  }

  public Vector4 BackgroundColor
  {
    get
    {
      Vector4 backgroundColor = new Vector4(0.0f,0.0f,0.0f,0.0f);

      Dali.Property.Map background = Background;
      int visualType = 0;
      background.Find( Dali.Constants.Visual.Property.Type ).Get( ref visualType );
      if(visualType == (int)Dali.Constants.Visual.Type.Color)
      {
        background.Find( Dali.Constants.ColorVisualProperty.MixColor ).Get( backgroundColor );
      }

      return backgroundColor;
    }
    set
    {
      SetProperty( View.Property.BACKGROUND, new Dali.Property.Value( value ) );
    }
  }

  public string BackgroundImage
  {
    get
    {
      string backgroundImage = "";

      Dali.Property.Map background = Background;
      int visualType = 0;
      background.Find( Dali.Constants.Visual.Property.Type ).Get( ref visualType );
      if(visualType == (int)Dali.Constants.Visual.Type.Image)
      {
        background.Find( Dali.Constants.ImageVisualProperty.URL ).Get( out backgroundImage );
      }

      return backgroundImage;
    }
    set
    {
      SetProperty( View.Property.BACKGROUND, new Dali.Property.Value( value ) );
    }
  }

  public bool KeyInputFocus
  {
    get
    {
      bool temp = false;
      GetProperty( View.Property.KEY_INPUT_FOCUS).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.KEY_INPUT_FOCUS, new Dali.Property.Value( value ) );
    }
  }

  public Dali.Property.Map Background
  {
    get
    {
      Dali.Property.Map temp = new Dali.Property.Map();
      GetProperty( View.Property.BACKGROUND).Get(  temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.BACKGROUND, new Dali.Property.Value( value ) );
    }
  }

  public string State
  {
    get
    {
      string temp;
      GetProperty( View.Property.STATE).Get( out temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.STATE, new Dali.Property.Value( value ) );
    }
  }

  public string SubState
  {
    get
    {
      string temp;
      GetProperty( View.Property.SUB_STATE).Get( out temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.SUB_STATE, new Dali.Property.Value( value ) );
    }
  }

  public Dali.Property.Map Tooltip
  {
    get
    {
      Dali.Property.Map temp = new Dali.Property.Map();
      GetProperty( View.Property.TOOLTIP).Get(  temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.TOOLTIP, new Dali.Property.Value( value ) );
    }
  }

  public string TooltipText
  {
    set
    {
      SetProperty( View.Property.TOOLTIP, new Dali.Property.Value( value ) );
    }
  }

  private int LeftFocusableActorId
  {
    get
    {
      int temp = 0;
      GetProperty( View.Property.LEFT_FOCUSABLE_ACTOR_ID).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.LEFT_FOCUSABLE_ACTOR_ID, new Dali.Property.Value( value ) );
    }
  }

  private int RightFocusableActorId
  {
    get
    {
      int temp = 0;
      GetProperty( View.Property.RIGHT_FOCUSABLE_ACTOR_ID).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.RIGHT_FOCUSABLE_ACTOR_ID, new Dali.Property.Value( value ) );
    }
  }

  private int UpFocusableActorId
  {
    get
    {
      int temp = 0;
      GetProperty( View.Property.UP_FOCUSABLE_ACTOR_ID).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.UP_FOCUSABLE_ACTOR_ID, new Dali.Property.Value( value ) );
    }
  }

  private int DownFocusableActorId
  {
    get
    {
      int temp = 0;
      GetProperty( View.Property.DOWN_FOCUSABLE_ACTOR_ID).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( View.Property.DOWN_FOCUSABLE_ACTOR_ID, new Dali.Property.Value( value ) );
    }
  }

  public float Flex
  {
    get
    {
      float temp = 0.0f;
      GetProperty( FlexContainer.ChildProperty.FLEX).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( FlexContainer.ChildProperty.FLEX, new Dali.Property.Value( value ) );
    }
  }

  public int AlignSelf
  {
    get
    {
      int temp = 0;
      GetProperty( FlexContainer.ChildProperty.ALIGN_SELF).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( FlexContainer.ChildProperty.ALIGN_SELF, new Dali.Property.Value( value ) );
    }
  }

  public Vector4 FlexMargin
  {
    get
    {
      Vector4 temp = new Vector4(0.0f,0.0f,0.0f,0.0f);
      GetProperty( FlexContainer.ChildProperty.FLEX_MARGIN).Get(  temp );
      return temp;
    }
    set
    {
      SetProperty( FlexContainer.ChildProperty.FLEX_MARGIN, new Dali.Property.Value( value ) );
    }
  }

  public Vector2 CellIndex
  {
    get
    {
      Vector2 temp = new Vector2(0.0f,0.0f);
      GetProperty( TableView.ChildProperty.CELL_INDEX).Get(  temp );
      return temp;
    }
    set
    {
      SetProperty( TableView.ChildProperty.CELL_INDEX, new Dali.Property.Value( value ) );
    }
  }

  public float RowSpan
  {
    get
    {
      float temp = 0.0f;
      GetProperty( TableView.ChildProperty.ROW_SPAN).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( TableView.ChildProperty.ROW_SPAN, new Dali.Property.Value( value ) );
    }
  }

  public float ColumnSpan
  {
    get
    {
      float temp = 0.0f;
      GetProperty( TableView.ChildProperty.COLUMN_SPAN).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( TableView.ChildProperty.COLUMN_SPAN, new Dali.Property.Value( value ) );
    }
  }

  public string CellHorizontalAlignment
  {
    get
    {
      string temp;
      GetProperty( TableView.ChildProperty.CELL_HORIZONTAL_ALIGNMENT).Get( out temp );
      return temp;
    }
    set
    {
      SetProperty( TableView.ChildProperty.CELL_HORIZONTAL_ALIGNMENT, new Dali.Property.Value( value ) );
    }
  }

  public string CellVerticalAlignment
  {
    get
    {
      string temp;
      GetProperty( TableView.ChildProperty.CELL_VERTICAL_ALIGNMENT).Get( out temp );
      return temp;
    }
    set
    {
      SetProperty( TableView.ChildProperty.CELL_VERTICAL_ALIGNMENT, new Dali.Property.Value( value ) );
    }
  }

  /**
   * @brief The left focusable view.
   * @note This will return NULL if not set.
   * This will also return NULL if the specified left focusable view is not on stage.
   *
   */
  public View LeftFocusableView
  {
    // As native side will be only storing IDs so need a logic to convert View to ID and vice-versa.
    get
    {
      if (LeftFocusableActorId >= 0)
      {
        return ConvertIdToView((uint)LeftFocusableActorId);
      }
      return null;
    }
    set
    {
      LeftFocusableActorId = (int)value.GetId();
    }
  }

  /**
   * @brief The right focusable view.
   * @note This will return NULL if not set.
   * This will also return NULL if the specified right focusable view is not on stage.
   *
   */
  public View RightFocusableView
  {
    // As native side will be only storing IDs so need a logic to convert View to ID and vice-versa.
    get
    {
      if (RightFocusableActorId >= 0)
      {
        return ConvertIdToView((uint)RightFocusableActorId);
      }
      return null;
    }
    set
    {
      RightFocusableActorId = (int)value.GetId();
    }
  }

  /**
   * @brief The up focusable view.
   * @note This will return NULL if not set.
   * This will also return NULL if the specified up focusable view is not on stage.
   *
   */
  public View UpFocusableView
  {
    // As native side will be only storing IDs so need a logic to convert View to ID and vice-versa.
    get
    {
      if (UpFocusableActorId >= 0)
      {
        return ConvertIdToView((uint)UpFocusableActorId);
      }
      return null;
    }
    set
    {
      UpFocusableActorId = (int)value.GetId();
    }
  }

  /**
   * @brief The down focusable view.
   * @note This will return NULL if not set.
   * This will also return NULL if the specified down focusable view is not on stage.
   *
   */
  public View DownFocusableView
  {
    // As native side will be only storing IDs so need a logic to convert View to ID and vice-versa.
    get
    {
      if (DownFocusableActorId >= 0)
      {
        return ConvertIdToView((uint)DownFocusableActorId);
      }
      return null;
    }
    set
    {
      DownFocusableActorId = (int)value.GetId();
    }
  }
}

}
