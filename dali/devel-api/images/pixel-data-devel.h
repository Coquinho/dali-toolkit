#ifndef DALI_PIXEL_DATA_DEVEL_H
#define DALI_PIXEL_DATA_DEVEL_H

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

// INTERNAL INCLUDES
#include <dali/public-api/images/pixel-data.h>

// INTERNAL INCLUDES
#include <dali/devel-api/images/pixel-devel.h>

namespace Dali
{

namespace DevelPixelData
{

/**
 * @brief Creates a PixelData object, using development pixel formats
 *
 * @param[in] buffer          The raw pixel data
 * @param[in] bufferSize      The size of the buffer in bytes
 * @param[in] width           Buffer width in pixels
 * @param[in] height          Buffer height in pixels
 * @param[in] pixelFormat     The pixel format
 * @param[in] releaseFunction The function used to release the memory
 * @return A handle to the PixelData
 */
DALI_IMPORT_API PixelData New( unsigned char* buffer,
                               unsigned int bufferSize,
                               unsigned int width,
                               unsigned int height,
                               DevelPixel::Format pixelFormat,
                               PixelData::ReleaseFunction releaseFunction );

/**
 * Struct to keep the buffer pointer and the allocation method.
 *
 * @note Need to find a better solution - one library should not
 * be freeing data from a different source with potentially
 * different allocators.
 */
struct PixelDataBuffer
{
  unsigned char* buffer;
  unsigned int bufferSize;
  PixelData::ReleaseFunction releaseFunction;

  PixelDataBuffer(  unsigned char* buffer,
                    unsigned int bufferSize,
                    PixelData::ReleaseFunction releaseFunction )
  : buffer(buffer),
    bufferSize(bufferSize),
    releaseFunction(releaseFunction)
  {
  }
};

/**
 * Get the buffer from a pixel data object, zero it in the pixel data object
 * and release the handle.
 * @param[in,out] pixelData The pixel data object to take the buffer from
 * @return the buffer and the data release mechanism
 */
PixelDataBuffer ReleasePixelDataBuffer( PixelData& pixelData );

} // namespace DevelPixelData

} // Dali

#endif // DALI_PIXEL_DATA_DEVEL_H