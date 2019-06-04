#ifndef DALI_V8PLUGIN_LAYER_API_H
#define DALI_V8PLUGIN_LAYER_API_H

/*
 * Copyright (c) 2019 Samsung Electronics Co., Ltd.
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
#include <v8.h>
#include <dali/public-api/actors/layer.h>

namespace Dali
{

namespace V8Plugin
{

namespace LayerApi
{
  /**
   * Layer constructor
   */
  Actor New( const v8::FunctionCallbackInfo< v8::Value >& args );

  /**
   * Layer API see layer.h for a description
   */
  void GetDepth( const v8::FunctionCallbackInfo< v8::Value >& args );
  void Raise( const v8::FunctionCallbackInfo< v8::Value >& args );
  void Lower( const v8::FunctionCallbackInfo< v8::Value >& args );
  void RaiseAbove( const v8::FunctionCallbackInfo< v8::Value >& args );
  void LowerBelow( const v8::FunctionCallbackInfo< v8::Value >& args );
  void RaiseToTop( const v8::FunctionCallbackInfo< v8::Value >& args );
  void LowerToBottom( const v8::FunctionCallbackInfo< v8::Value >& args );
  void MoveBelow( const v8::FunctionCallbackInfo< v8::Value >& args );
  void MoveAbove( const v8::FunctionCallbackInfo< v8::Value >& args );
  void SetDepthTestDisabled( const v8::FunctionCallbackInfo< v8::Value >& args );
  void IsDepthTestDisabled( const v8::FunctionCallbackInfo< v8::Value >& args );


}; // namespace LayerApi

} // namespace V8Plugin

} // namespace Dali

#endif // DALI_V8PLUGIN_LAYER_API_H
