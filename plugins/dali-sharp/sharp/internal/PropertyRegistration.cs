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

public class PropertyRegistration : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal PropertyRegistration(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(PropertyRegistration obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~PropertyRegistration() {
    Dispose();
  }

  public virtual void Dispose() {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          NDalicPINVOKE.delete_PropertyRegistration(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      global::System.GC.SuppressFinalize(this);
    }
  }

  public PropertyRegistration(TypeRegistration registered, string name, int index, Property.Type type, SWIGTYPE_p_f_p_Dali__BaseObject_int_r_q_const__Dali__Property__Value__void setFunc, SWIGTYPE_p_f_p_Dali__BaseObject_Dali__Property__Index__Dali__Property__Value getFunc) : this(NDalicPINVOKE.new_PropertyRegistration(TypeRegistration.getCPtr(registered), name, index, (int)type, SWIGTYPE_p_f_p_Dali__BaseObject_int_r_q_const__Dali__Property__Value__void.getCPtr(setFunc), SWIGTYPE_p_f_p_Dali__BaseObject_Dali__Property__Index__Dali__Property__Value.getCPtr(getFunc)), true) {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

}

}
