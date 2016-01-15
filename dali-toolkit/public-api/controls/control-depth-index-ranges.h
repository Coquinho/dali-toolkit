#ifndef __DALI_TOOLKIT_CONTROL_DEPTH_INDEX_RANGES_H__
#define __DALI_TOOLKIT_CONTROL_DEPTH_INDEX_RANGES_H__

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
#include <dali/public-api/actors/layer.h>

namespace Dali
{
namespace Toolkit
{
/**
 * @addtogroup dali_toolkit_controls
 * @{
 */

/**
 * @deprecated DALi 1.1.16
 *
 * @brief These depth indices should not be used.
 */
enum DepthIndexRanges
{
    BACKGROUND_DEPTH_INDEX    = static_cast<int>( -Dali::Layer::TREE_DEPTH_MULTIPLIER * 0.1f ), ///< @deprecated DALi 1.1.16
    CONTENT_DEPTH_INDEX       = 0,                                                              ///< @deprecated DALi 1.1.16
    DECORATION_DEPTH_INDEX    = static_cast<int>( Dali::Layer::TREE_DEPTH_MULTIPLIER * 0.1f )   ///< @deprecated DALi 1.1.16
};

/**
 * @}
 */
}

}

#endif
