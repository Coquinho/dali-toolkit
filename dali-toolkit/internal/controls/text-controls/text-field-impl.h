#ifndef __DALI_TOOLKIT_INTERNAL_TEXT_FIELD_H__
#define __DALI_TOOLKIT_INTERNAL_TEXT_FIELD_H__

/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

// INTERNAL INCLUDES
#include <dali-toolkit/public-api/controls/control-impl.h>
#include <dali-toolkit/public-api/controls/text-controls/text-field.h>
#include <dali-toolkit/internal/text/decorator/text-decorator.h>
#include <dali-toolkit/internal/text/text-control-interface.h>
#include <dali-toolkit/internal/text/text-controller.h>
#include <dali-toolkit/internal/text/rendering/text-renderer.h>

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

/**
 * @brief A control which renders a short text string.
 */
class TextField : public Control, public Text::ControlInterface
{
public:

  /**
   * @copydoc Dali::Toollkit::TextField::New()
   */
  static Toolkit::TextField New();

  // Properties

  /**
   * Called when a property of an object of this type is set.
   * @param[in] object The object whose property is set.
   * @param[in] index The property index.
   * @param[in] value The new property value.
   */
  static void SetProperty( BaseObject* object, Property::Index index, const Property::Value& value );

  /**
   * Called to retrieve a property of an object of this type.
   * @param[in] object The object whose property is to be retrieved.
   * @param[in] index The property index.
   * @return The current value of the property.
   */
  static Property::Value GetProperty( BaseObject* object, Property::Index index );

private: // From Control

  /**
   * @copydoc Control::OnInitialize()
   */
  virtual void OnInitialize();

  /**
   * @copydoc Control::GetNaturalSize()
   */
  virtual Vector3 GetNaturalSize();

  /**
   * @copydoc Control::GetHeightForWidth()
   */
  virtual float GetHeightForWidth( float width );

  /**
   * @copydoc Control::OnInitialize()
   */
  virtual void OnRelayout( const Vector2& size, ActorSizeContainer& container );

  /**
   * Received for single & double taps
   */
  virtual void OnTap( const TapGesture& tap );

  /**
   * @copydoc Dali::CustomActorImpl::OnKeyEvent(const KeyEvent&)
   */
  virtual bool OnKeyEvent(const KeyEvent& event);

  /**
   * @copydoc Text::ControlInterface::RequestTextRelayout()
   */
  virtual void RequestTextRelayout();

private: // Implementation

  /**
   * Construct a new TextField.
   */
  TextField();

  /**
   * A reference counted object may only be deleted by calling Unreference()
   */
  virtual ~TextField();

private:

  // Undefined copy constructor and assignment operators
  TextField(const TextField&);
  TextField& operator=(const TextField& rhs);

private: // Data

  Text::ControllerPtr mController;
  Text::RendererPtr mRenderer;
  Text::DecoratorPtr mDecorator;

  unsigned int mRenderingBackend;
};

} // namespace Internal

// Helpers for public-api forwarding methods

inline Toolkit::Internal::TextField& GetImpl( Toolkit::TextField& textField )
{
  DALI_ASSERT_ALWAYS(textField);

  Dali::RefObject& handle = textField.GetImplementation();

  return static_cast<Toolkit::Internal::TextField&>(handle);
}

inline const Toolkit::Internal::TextField& GetImpl( const Toolkit::TextField& textField )
{
  DALI_ASSERT_ALWAYS(textField);

  const Dali::RefObject& handle = textField.GetImplementation();

  return static_cast<const Toolkit::Internal::TextField&>(handle);
}

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_INTERNAL_TEXT_FIELD_H__
