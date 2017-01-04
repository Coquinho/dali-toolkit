/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
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

#include "dummy-control.h"

#include <dali-toolkit/dali-toolkit.h>
#include <dali-toolkit/devel-api/visual-factory/visual-factory.h>
#include <dali-toolkit/devel-api/visuals/visual-properties-devel.h>
#include <dali-toolkit/devel-api/align-enums.h>

namespace Dali
{

namespace Toolkit
{

DummyControl::DummyControl()
{
}

DummyControl::DummyControl(const DummyControl& control)
: Control( control )
{
}

DummyControl::~DummyControl()
{
}

DummyControl DummyControl::DownCast( BaseHandle handle )
{
  return Control::DownCast<DummyControl, DummyControlImpl>(handle);
}

DummyControl& DummyControl::operator=(const DummyControl& control)
{
  Control::operator=( control );
  return *this;
}

// Used to test signal connections
void DummyControlImpl::CustomSlot1( Actor actor )
{
  mCustomSlot1Called = true;
}

namespace {

BaseHandle Create()
{
  return DummyControlImpl::New();
}

DALI_TYPE_REGISTRATION_BEGIN( Toolkit::DummyControl, Toolkit::Control, Create );
DALI_TYPE_REGISTRATION_END()

Dali::PropertyRegistration dummyControlVisualProperty(
  typeRegistration, "testVisual", Dali::Toolkit::DummyControl::Property::TEST_VISUAL, Dali::Property::MAP, &Dali::Toolkit::DummyControlImpl::SetProperty, &Dali::Toolkit::DummyControlImpl::GetProperty );

}

DummyControl DummyControlImpl::New()
{
  IntrusivePtr< DummyControlImpl > impl = new DummyControlImpl;
  DummyControl control( *impl );
  impl->Initialize();
  return control;
}

DummyControlImpl::DummyControlImpl()
: Control( ControlBehaviour( REQUIRES_TOUCH_EVENTS | REQUIRES_HOVER_EVENTS | REQUIRES_STYLE_CHANGE_SIGNALS ) ),
  mCustomSlot1Called(false)
{
}

DummyControlImpl::~DummyControlImpl()
{
}

void DummyControlImpl::RegisterVisual( Property::Index index, Toolkit::Visual::Base visual )
{
  Control::RegisterVisual( index, visual );

  VisualIndices::iterator iter = std::find( mRegisteredVisualIndices.begin(), mRegisteredVisualIndices.end(), index );
  if( iter == mRegisteredVisualIndices.end() )
  {
    mRegisteredVisualIndices.push_back(index);
  }
}

void DummyControlImpl::RegisterVisual( Property::Index index, Toolkit::Visual::Base visual, bool enabled )
{
  Control::RegisterVisual( index, visual, enabled );

  VisualIndices::iterator iter = std::find( mRegisteredVisualIndices.begin(), mRegisteredVisualIndices.end(), index );
  if( iter == mRegisteredVisualIndices.end() )
  {
    mRegisteredVisualIndices.push_back(index);
  }
}

void DummyControlImpl::UnregisterVisual( Property::Index index )
{
  Control::UnregisterVisual( index );
}

Toolkit::Visual::Base DummyControlImpl::GetVisual( Property::Index index )
{
  return Control::GetVisual( index );
}

void DummyControlImpl::EnableVisual( Property::Index index, bool enabled )
{
  Control::EnableVisual( index, enabled );
}

bool DummyControlImpl::IsVisualEnabled( Property::Index index )
{
  return Control::IsVisualEnabled( index );
}


Animation DummyControlImpl::CreateTransition( const Toolkit::TransitionData& transition )
{
  return Control::CreateTransition( transition );
}

void DummyControlImpl::SetProperty( BaseObject* object, Dali::Property::Index index, const Dali::Property::Value& value )
{
}

Property::Value DummyControlImpl::GetProperty( BaseObject* object, Dali::Property::Index propertyIndex )
{
  Dali::Property::Value value;
  return value;
}



DummyControl DummyControlImplOverride::New()
{
  IntrusivePtr< DummyControlImplOverride > impl = new DummyControlImplOverride;
  DummyControl control( *impl );
  impl->Initialize();
  return control;
}


DummyControlImplOverride::DummyControlImplOverride()
: DummyControlImpl(),
  initializeCalled(false),
  activatedCalled(false),
  onAccTouchedCalled(false),
  onAccValueChangeCalled(false),
  themeChangeCalled(false),
  fontChangeCalled(false),
  pinchCalled(false),
  panCalled(false),
  tapCalled(false),
  longPressCalled(false),
  stageConnectionCalled(false),
  stageDisconnectionCalled(false),
  childAddCalled(false),
  childRemoveCalled(false),
  sizeSetCalled(false),
  sizeAnimationCalled(false),
  touchEventCalled(false),
  hoverEventCalled(false),
  wheelEventCalled(false),
  keyEventCalled(false),
  keyInputFocusGained(false),
  keyInputFocusLost(false)
{
}

DummyControlImplOverride::~DummyControlImplOverride() { }


void DummyControlImplOverride::OnInitialize() { initializeCalled = true; }
bool DummyControlImplOverride::OnAccessibilityActivated() { activatedCalled = true; return true; }
bool DummyControlImplOverride::OnAccessibilityTouch(const TouchEvent& touchEvent) { onAccTouchedCalled = true; return true; }
bool DummyControlImplOverride::OnAccessibilityValueChange( bool isIncrease )
{
  onAccValueChangeCalled = true; return true;
}

void DummyControlImplOverride::OnStyleChange( Toolkit::StyleManager styleManager, StyleChange::Type change )
{
  themeChangeCalled = change == StyleChange::THEME_CHANGE;
  fontChangeCalled = change == StyleChange::DEFAULT_FONT_SIZE_CHANGE;
}
void DummyControlImplOverride::OnPinch(const PinchGesture& pinch) { pinchCalled = true; }
void DummyControlImplOverride::OnPan(const PanGesture& pan) { panCalled = true; }
void DummyControlImplOverride::OnTap(const TapGesture& tap) { tapCalled = true; }
void DummyControlImplOverride::OnLongPress(const LongPressGesture& longPress) { longPressCalled = true; }
void DummyControlImplOverride::OnStageConnection( int depth ) { Control::OnStageConnection( depth ); stageConnectionCalled = true; }
void DummyControlImplOverride::OnStageDisconnection() { stageDisconnectionCalled = true; Control::OnStageDisconnection(); }
void DummyControlImplOverride::OnChildAdd(Actor& child) { childAddCalled = true; }
void DummyControlImplOverride::OnChildRemove(Actor& child) { childRemoveCalled = true; }
void DummyControlImplOverride::OnSizeSet(const Vector3& targetSize) { Control::OnSizeSet( targetSize ); sizeSetCalled = true; }
void DummyControlImplOverride::OnSizeAnimation(Animation& animation, const Vector3& targetSize) { Control::OnSizeAnimation( animation, targetSize ); sizeAnimationCalled = true; }
bool DummyControlImplOverride::OnTouchEvent(const TouchEvent& event) { touchEventCalled = true; return false; }
bool DummyControlImplOverride::OnHoverEvent(const HoverEvent& event) { hoverEventCalled = true; return false; }
bool DummyControlImplOverride::OnWheelEvent(const WheelEvent& event) { wheelEventCalled = true; return false; }
bool DummyControlImplOverride::OnKeyEvent(const KeyEvent& event) { keyEventCalled = true; return false;}
void DummyControlImplOverride::OnKeyInputFocusGained() { keyInputFocusGained = true; }
void DummyControlImplOverride::OnKeyInputFocusLost() { keyInputFocusLost = true; }

void DummyControlImplOverride::OnRelayout( const Vector2& size, RelayoutContainer& container )
{
  Property::Map transformMap;
  transformMap
    .Add( Toolkit::DevelVisual::Transform::Property::OFFSET, Vector2(0.0f, 0.0f) )
    .Add( Toolkit::DevelVisual::Transform::Property::SIZE, Vector2(1.0f, 1.0f) )
    .Add( Toolkit::DevelVisual::Transform::Property::ORIGIN, Toolkit::Align::CENTER )
    .Add( Toolkit::DevelVisual::Transform::Property::ANCHOR_POINT, Toolkit::Align::CENTER )
    .Add( Toolkit::DevelVisual::Transform::Property::OFFSET_SIZE_MODE, Vector4::ZERO );

  for( VisualIndices::iterator iter = mRegisteredVisualIndices.begin(); iter != mRegisteredVisualIndices.end() ; ++iter )
  {
    Visual::Base visual = GetVisual(*iter);
    visual.SetTransformAndSize( transformMap, size );
  }
}


DummyControl DummyControl::New( bool override )
{
  DummyControl control;

  if (override)
  {
    control = DummyControlImplOverride::New();
  }
  else
  {
    control = DummyControlImpl::New();
  }

  return control;
}

DummyControl::DummyControl( DummyControlImpl& implementation )
: Control( implementation )
{
}

DummyControl::DummyControl( Dali::Internal::CustomActor* internal )
: Control( internal )
{
  VerifyCustomActorPointer<DummyControlImpl>(internal);
}

} // namespace Toolkit

} // namespace Dali
