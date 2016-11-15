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

%define ACTOR_EVENTHANDLER_TYPEMAP_EVENTARG(NameSpace, ClassName)
%typemap(csimports) NameSpace::ClassName %{
    using System;
    using System.Runtime.InteropServices;
%}

%enddef

%define ACTOR_EVENTHANDLER_TYPEMAP_HELPER(NameSpace, ClassName)
  %typemap(cscode) NameSpace::ClassName %{

    public static ClassName Get ## ClassName ## FromPtr(global::System.IntPtr cPtr) {
      ClassName ret = new ClassName(cPtr, false);
      if (NDalicPINVOKE.SWIGPendingException.Pending) throw NDalicPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    }

    public IntPtr GetPtrfrom ## ClassName ()
    {
       return (IntPtr)swigCPtr;
    }

    public Actor Parent
    {
      get
      {
        Actor parent = GetParent();
        return parent;
      }
    }

   public bool Visibility
   {
      get
      {
        bool visibility = IsVisible();
        return visibility;
      }
    }

   public float Opacity
   {
      set
      {
        SetOpacity(value);
      }
      get
      {
        float ret = GetCurrentOpacity();
        return ret;
      }
   }

    public bool StateFocusEnable
    {
      set
      {
        SetKeyboardFocusable(value);
      }
      get
      {
        bool focusable = IsKeyboardFocusable();
        return focusable;
      }
    }

    public void Show()
    {
      SetVisible(true);
    }

    public void Hide()
    {
      SetVisible(false);
    }

%}

%enddef

%define DALI_ACTOR_EVENTHANDLER_PARAM( NameSpace, ClassName)

ACTOR_EVENTHANDLER_TYPEMAP_EVENTARG( NameSpace, ClassName);
ACTOR_EVENTHANDLER_TYPEMAP_HELPER( NameSpace, ClassName);

%enddef

namespace Dali
{
  DALI_ACTOR_EVENTHANDLER_PARAM( Dali, Actor);
}
