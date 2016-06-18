#ifndef __DALI_TOOLKIT_INTERNAL_TEXT_EFFECTS_STYLE_H__
#define __DALI_TOOLKIT_INTERNAL_TEXT_EFFECTS_STYLE_H__

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

// EXTERNAL INCLUDES
#include <dali/devel-api/scripting/scripting.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/text/text-controller.h>
#include <dali-toolkit/internal/text/text-definitions.h>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

namespace EffectStyle
{
  enum Type
  {
    DEFAULT, ///< The default text effect style.
    INPUT    ///< The input text effect style.
  };
};

/**
 * @brief Sets the underline properties.
 *
 * @param[in] controller The text's controller.
 * @param[in] value The values of the underline's properties.
 * @param[in] type Whether the property is for the default underline or the input underline.
 *
 * @return Whether the underline properties have been updated.
 */
bool SetUnderlineProperties( ControllerPtr controller, const Property::Value& value, EffectStyle::Type type );

/**
 * @brief Retrieves the underline's properties.
 *
 * @param[in] controller The text's controller.
 * @param[out] value The value of the underline's properties.
 * @param[in] type Whether the property is for the default underline or the input underline.
 */
void GetUnderlineProperties( ControllerPtr controller, Property::Value& value, EffectStyle::Type type );

/**
 * @brief Sets the shadow properties.
 *
 * @param[in] controller The text's controller.
 * @param[in] value The values of the shadow's style.
 * @param[in] type Whether the property is for the default shadow's style or the input shadow's style.
 *
 * @return Whether the shadow properties have been updated.
 */
bool SetShadowProperties( ControllerPtr controller, const Property::Value& value, EffectStyle::Type type );

/**
 * @brief Retrieves the shadow's properties.
 *
 * @param[in] controller The text's controller.
 * @param[out] value The value of the shadow's properties.
 * @param[in] type Whether the property is for the default shadow or the input shadow.
 */
void GetShadowProperties( ControllerPtr controller, Property::Value& value, EffectStyle::Type type );

/**
 * @brief Sets the emboss properties.
 *
 * @param[in] controller The text's controller.
 * @param[in] value The values of the emboss's properties.
 * @param[in] type Whether the property is for the default emboss or the input emboss.
 *
 * @return Whether the emboss properties have been updated.
 */
bool SetEmbossProperties( ControllerPtr controller, const Property::Value& value, EffectStyle::Type type );

/**
 * @brief Retrieves the emboss's properties.
 *
 * @param[in] controller The text's controller.
 * @param[out] value The value of the emboss's properties.
 * @param[in] type Whether the property is for the default emboss or the input emboss.
 */
void GetEmbossProperties( ControllerPtr controller, Property::Value& value, EffectStyle::Type type );

/**
 * @brief Sets the outline properties.
 *
 * @param[in] controller The text's controller.
 * @param[in] value The values of the outline's properties.
 * @param[in] type Whether the property is for the default outline or the input outline.
 *
 * @return Whether the outline properties have been updated.
 */
bool SetOutlineProperties( ControllerPtr controller, const Property::Value& value, EffectStyle::Type type );

/**
 * @brief Retrieves the outline's properties.
 *
 * @param[in] controller The text's controller.
 * @param[out] value The value of the outline's properties.
 * @param[in] type Whether the property is for the default outline or the input outline.
 */
void GetOutlineProperties( ControllerPtr controller, Property::Value& value, EffectStyle::Type type );

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_INTERNAL_TEXT_EFFECTS_STYLE_H__
