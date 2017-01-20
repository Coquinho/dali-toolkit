/*
 * Copyright (c) 2017 Samsung Electronics Co., Ltd.
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

%typemap(cscode) Dali::Actor::Property %{
  public static readonly int BATCH_PARENT = NDalicManualPINVOKE.Actor_Property_BATCH_PARENT_get();
  public static readonly int SIBLING_ORDER = NDalicManualPINVOKE.Actor_Property_SIBLING_ORDER_get();
%}

%typemap(cscode) Dali::Renderer::Property %{
  public static readonly int BATCHING_ENABLED = NDalicManualPINVOKE.Renderer_Property_BATCHING_ENABLED_get();
%}

%typemap(cscode) Dali::Renderer %{
  public bool BatchingEnabled
  {
    get
    {
      bool temp = false;
      GetProperty( Renderer.Property.BATCHING_ENABLED).Get( ref temp );
      return temp;
    }
    set
    {
      SetProperty( Renderer.Property.BATCHING_ENABLED, new Dali.Property.Value( value ) );
    }
  }
%}

%typemap(cscode) Dali::Toolkit::Control::Property %{
  public static readonly int TOOLTIP = NDalicManualPINVOKE.View_Property_TOOLTIP_get();
%}

%typemap(cscode) Dali::Toolkit::ItemView::Property %{
  public static readonly int LAYOUT = NDalicManualPINVOKE.ItemView_Property_LAYOUT_get();
%}

%typemap(cscode) Dali::Toolkit::ItemView %{
  public Dali.Property.Array Layout
  {
    get
    {
      Dali.Property.Array temp = new Dali.Property.Array();
      GetProperty( ItemView.Property.LAYOUT).Get( temp );
      return temp;
    }
    set
    {
      SetProperty( ItemView.Property.LAYOUT, new Dali.Property.Value( value ) );
    }
  }
%}

%typemap(cscode) Dali::Toolkit::Button::Property %{
  public static readonly int UNSELECTED_VISUAL = NDalicManualPINVOKE.Button_Property_UNSELECTED_VISUAL_get();
  public static readonly int SELECTED_VISUAL = NDalicManualPINVOKE.Button_Property_SELECTED_VISUAL_get();
  public static readonly int DISABLED_SELECTED_VISUAL = NDalicManualPINVOKE.Button_Property_DISABLED_SELECTED_VISUAL_get();
  public static readonly int DISABLED_UNSELECTED_VISUAL = NDalicManualPINVOKE.Button_Property_DISABLED_UNSELECTED_VISUAL_get();
  public static readonly int UNSELECTED_BACKGROUND_VISUAL = NDalicManualPINVOKE.Button_Property_UNSELECTED_BACKGROUND_VISUAL_get();
  public static readonly int SELECTED_BACKGROUND_VISUAL = NDalicManualPINVOKE.Button_Property_SELECTED_BACKGROUND_VISUAL_get();
  public static readonly int DISABLED_UNSELECTED_BACKGROUND_VISUAL = NDalicManualPINVOKE.Button_Property_DISABLED_UNSELECTED_BACKGROUND_VISUAL_get();
  public static readonly int DISABLED_SELECTED_BACKGROUND_VISUAL = NDalicManualPINVOKE.Button_Property_DISABLED_SELECTED_BACKGROUND_VISUAL_get();
  public static readonly int LABEL_RELATIVE_ALIGNMENT = NDalicManualPINVOKE.Button_Property_LABEL_RELATIVE_ALIGNMENT_get();
  public static readonly int LABEL_PADDING = NDalicManualPINVOKE.Button_Property_LABEL_PADDING_get();
  public static readonly int VISUAL_PADDING = NDalicManualPINVOKE.Button_Property_VISUAL_PADDING_get();
%}

