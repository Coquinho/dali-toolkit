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

public class CustomActor : Actor {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;

  internal CustomActor(global::System.IntPtr cPtr, bool cMemoryOwn) : base(NDalicPINVOKE.CustomActor_SWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(CustomActor obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~CustomActor() {
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
          NDalicPINVOKE.delete_CustomActor(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
      base.Dispose();
    }
  }


  public CustomActor() : this(NDalicPINVOKE.new_CustomActor__SWIG_0(), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public new static CustomActor DownCast(BaseHandle handle) {
    CustomActor ret = new CustomActor(NDalicPINVOKE.CustomActor_DownCast(BaseHandle.getCPtr(handle)), true);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public CustomActorImpl GetImplementation() {
    CustomActorImpl ret = new CustomActorImpl(NDalicPINVOKE.CustomActor_GetImplementation(swigCPtr), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public CustomActor(CustomActorImpl implementation) : this(NDalicPINVOKE.new_CustomActor__SWIG_1(CustomActorImpl.getCPtr(implementation)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public CustomActor(CustomActor copy) : this(NDalicPINVOKE.new_CustomActor__SWIG_2(CustomActor.getCPtr(copy)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public CustomActor Assign(CustomActor rhs) {
    CustomActor ret = new CustomActor(NDalicPINVOKE.CustomActor_Assign(swigCPtr, CustomActor.getCPtr(rhs)), false);
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}

}
