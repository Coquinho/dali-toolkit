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

%typemap(cscode) Dali::AlphaFunction %{
  public AlphaFunction(Dali.Constants.AlphaFunction.BuiltinFunction function) : this(NDalicPINVOKE.new_AlphaFunction__SWIG_1((int)function), true)
  {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }

  public AlphaFunction(System.Delegate func) : this(NDalicPINVOKE.new_AlphaFunction__SWIG_2(SWIGTYPE_p_f_float__float.getCPtr(new SWIGTYPE_p_f_float__float(System.Runtime.InteropServices.Marshal.GetFunctionPointerForDelegate(func), true))), true)
  {
    if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
  }
%}



