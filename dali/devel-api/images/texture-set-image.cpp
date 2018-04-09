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

// CLASS HEADER
#include "texture-set-image.h"

// INTERNAL INCLUDES
#include <dali/internal/event/rendering/texture-set-impl.h>
#include <dali/internal/event/images/image-impl.h>

namespace Dali
{

void TextureSetImage( TextureSet textureSet, size_t index, Image image )
{
  Internal::Texture* texture = NULL;
  if( image )
  {
    texture = GetImplementation( image ).GetTexture();
  }

  GetImplementation( textureSet ).SetTexture( index, texture );
}

} // namespace Dali