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
 */

// CLASS HEADER
#include <dali-toolkit/internal/visuals/animated-image/rolling-image-cache.h>

// INTERNAL HEADERS
#include <dali/integration-api/debug.h>

// EXTERNAL HEADERS

namespace
{
#if defined(DEBUG_ENABLED)
Debug::Filter* gAnimImgLogFilter = Debug::Filter::New(Debug::NoLogging, false, "LOG_ANIMATED_IMAGE");

#define LOG_CACHE                                                       \
  {                                                                     \
    std::ostringstream oss;                                             \
    oss<<"Size:"<<mQueue.Count()<<" [ ";                                \
    for(std::size_t _i=0; _i<mQueue.Count(); ++_i)                      \
    {                                                                   \
      oss<<_i<<                                                         \
        "={ tex:"<<mImageUrls[mQueue[_i].mUrlIndex].mTextureId<<        \
        " urlId:"<<mQueue[_i].mUrlIndex<<                               \
        " rdy:"<<(mQueue[_i].mReady?"T":"F")<< "}, ";                   \
    }                                                                   \
    oss<<" ]"<<std::endl;                                               \
    DALI_LOG_INFO(gAnimImgLogFilter,Debug::Concise,"%s",oss.str().c_str()); \
  }

#else
  #define LOG_CACHE
#endif

const bool ENABLE_ORIENTATION_CORRECTION( true );

}

namespace Dali
{
namespace Toolkit
{
namespace Internal
{

RollingImageCache::RollingImageCache(
  TextureManager& textureManager, UrlList& urlList, ImageCache::FrameReadyObserver& observer,
  uint16_t cacheSize, uint16_t batchSize )
: ImageCache( textureManager, urlList, observer, batchSize ),
  mQueue( cacheSize )
{
  LoadBatch();
}

RollingImageCache::~RollingImageCache()
{
  while( !mQueue.IsEmpty() )
  {
    ImageFrame imageFrame = mQueue.PopFront();
    mTextureManager.Remove( mImageUrls[ imageFrame.mUrlIndex ].mTextureId );
  }
}

TextureSet RollingImageCache::FirstFrame()
{
  TextureSet textureSet = GetFrontTextureSet();

  if( ! textureSet )
  {
    mWaitingForReadyFrame = true;
  }

  return textureSet;
}

TextureSet RollingImageCache::NextFrame()
{
  TextureSet textureSet;

  ImageFrame imageFrame = mQueue.PopFront();
  mTextureManager.Remove( mImageUrls[ imageFrame.mUrlIndex ].mTextureId );
  mImageUrls[ imageFrame.mUrlIndex ].mTextureId = TextureManager::INVALID_TEXTURE_ID;

  if( IsFrontReady() == true )
  {
    textureSet = GetFrontTextureSet();
  }
  else
  {
    mWaitingForReadyFrame = true;
  }

  LoadBatch();

  return textureSet;
}

bool RollingImageCache::IsFrontReady() const
{
  return ( !mQueue.IsEmpty() && mQueue.Front().mReady );
}

void RollingImageCache::LoadBatch()
{
  // Try and load up to mBatchSize images, until the cache is filled.
  // Once the cache is filled, as frames progress, the old frame is
  // cleared, but not erased, and another image is loaded
  bool frontFrameReady = IsFrontReady();;

  for( unsigned int i=0; i< mBatchSize && !mQueue.IsFull(); ++i )
  {
    ImageFrame imageFrame;

    std::string& url = mImageUrls[ mUrlIndex ].mUrl;
    imageFrame.mUrlIndex = mUrlIndex;
    imageFrame.mReady = false;

    ++mUrlIndex;
    mUrlIndex %= mImageUrls.size();

    mQueue.PushBack( imageFrame );

    // Note, if the image is already loaded, then UploadComplete will get called
    // from within this method. This means it won't yet have a texture id, so we
    // need to account for this inside the UploadComplete method using mRequestingLoad.
    mRequestingLoad = true;

    mImageUrls[ imageFrame.mUrlIndex ].mTextureId =
      mTextureManager.RequestLoad( url, ImageDimensions(), FittingMode::SCALE_TO_FILL,
                                   SamplingMode::BOX_THEN_LINEAR, TextureManager::NO_ATLAS,
                                   this, ENABLE_ORIENTATION_CORRECTION );
    mRequestingLoad = false;
  }

  CheckFrontFrame( frontFrameReady );
}

void RollingImageCache::SetImageFrameReady( TextureManager::TextureId textureId )
{
  for( std::size_t i = 0; i < mQueue.Count() ; ++i )
  {
    if( GetCachedTextureId(i) == textureId )
    {
      mQueue[i].mReady = true;
      break;
    }
  }
}

TextureSet RollingImageCache::GetFrontTextureSet() const
{
  TextureManager::TextureId textureId = GetCachedTextureId( 0 );
  return mTextureManager.GetTextureSet( textureId );
}

TextureManager::TextureId RollingImageCache::GetCachedTextureId( int index ) const
{
  return mImageUrls[ mQueue[ index ].mUrlIndex ].mTextureId;
}

void RollingImageCache::CheckFrontFrame( bool wasReady )
{
  if( mWaitingForReadyFrame && wasReady == false && IsFrontReady() )
  {
    mWaitingForReadyFrame = false;
    mObserver.FrameReady( GetFrontTextureSet() );
  }
}

void RollingImageCache::UploadComplete(
  bool           loadSuccess,
  int32_t        textureId,
  TextureSet     textureSet,
  bool           useAtlasing,
  const Vector4& atlasRect )
{
  DALI_LOG_INFO(gAnimImgLogFilter,Debug::Concise,"AnimatedImageVisual::UploadComplete(textureId:%d) start\n", textureId);
  LOG_CACHE;

  bool frontFrameReady = IsFrontReady();

  if( ! mRequestingLoad )
  {
    SetImageFrameReady( textureId );

    CheckFrontFrame( frontFrameReady );
  }
  else
  {
    // UploadComplete has been called from within RequestLoad. TextureManager must
    // therefore already have the texture cached, so make the texture ready.
    // (Use the last texture, as the texture id hasn't been assigned yet)
    mQueue.Back().mReady = true;
  }

  LOG_CACHE;
}

} //namespace Internal
} //namespace Toolkit
} //namespace Dali