#ifndef DALI_TOOLKIT_CONTROL_DEVEL_H
#define DALI_TOOLKIT_CONTROL_DEVEL_H

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

// INTERNAL INCLUDES
#include <dali-toolkit/public-api/controls/control.h>

namespace Dali
{

namespace Toolkit
{

namespace DevelControl
{

enum State
{
  NORMAL,
  FOCUSED,
  DISABLED
};

namespace Property
{

enum
{
  STYLE_NAME        = Control::Property::STYLE_NAME,
  BACKGROUND_COLOR  = Control::Property::BACKGROUND_COLOR,
  BACKGROUND_IMAGE  = Control::Property::BACKGROUND_IMAGE,
  KEY_INPUT_FOCUS   = Control::Property::KEY_INPUT_FOCUS,
  BACKGROUND        = Control::Property::BACKGROUND,

  /**
   * @brief Displays a tooltip when the control is hovered over.
   * @details Name "tooltip", type Property::STRING, Property::ARRAY or Property::MAP.
   *          If Property::STRING, then the style specified in the stylesheet is used.
   *          If Property::ARRAY of Visuals then all are displayed in one row.
   *          If Property::MAP, then it should be a map of Tooltip properties.
   * @note The tooltip is only activated if display content is received, i.e. a string (text) or visual to show.
   *       The rest is used to just build up the style of the tooltip (i.e. background, text color/point-size etc.)
   * @note When retrieved, a Property::MAP is returned.
   * @see Toolkit::Tooltip
   */
  TOOLTIP = BACKGROUND + 1,

  /**
   * @brief The current state of the control.
   * @details Name "state", type DevelControl::State ( Property::INTEGER ) or Property::STRING
   *
   * @see DevelControl::State
   */
  STATE = BACKGROUND + 2,

  /**
   * @brief The current sub state of the control.
   * @details Name "subState", type Property::INTEGER or Property::STRING. The enumeration used is dependent on the derived control.
   *
   * @see DevelControl::State
   */
  SUB_STATE = BACKGROUND + 3,

  /**
   * @brief The actor ID of the left focusable control.
   * @details Name "leftFocusableActorId", type Property::INTEGER.
   *
   */
  LEFT_FOCUSABLE_ACTOR_ID = BACKGROUND + 4,

  /**
   * @brief The actor ID of the right focusable control.
   * @details Name "rightFocusableActorId", type Property::INTEGER.
   *
   */
  RIGHT_FOCUSABLE_ACTOR_ID = BACKGROUND + 5,

  /**
   * @brief The actor ID of the up focusable control.
   * @details Name "upFocusableActorId", type Property::INTEGER.
   *
   */
  UP_FOCUSABLE_ACTOR_ID = BACKGROUND + 6,

  /**
   * @brief The actor ID of the down focusable control.
   * @details Name "downFocusableActorId", type Property::INTEGER.
   *
   */
  DOWN_FOCUSABLE_ACTOR_ID = BACKGROUND + 7
};

} // namespace Property

/// @brief ResourceReady signal type;
typedef Signal<void ( Control ) > ResourceReadySignalType;

/**
 * @brief This signal is emitted after all resources required
 * by a control are loaded and ready.
 * Most resources are only loaded when the control is placed on stage.
 *
 * A callback of the following type may be connected:
 * @code
 *   void YourCallbackName( Control control );
 * @endcode
 */
ResourceReadySignalType& ResourceReadySignal( Control& control );

/**
 * @brief Query if all resources required by a control are loaded and ready.
 * Most resources are only loaded when the control is placed on stage.
 * @return true if the resources are loaded and ready, false otherwise
 *
 */
bool IsResourceReady( const Control& control );

} // namespace DevelControl

} // namespace Toolkit

} // namespace Dali

#endif // DALI_TOOLKIT_CONTROL_DEVEL_H
