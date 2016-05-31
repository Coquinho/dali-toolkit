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

// CLASS HEADER
#include "image-renderer.h"

// EXTERNAL HEADER
#include <cstring> // for strncasecmp
#include <dali/public-api/images/resource-image.h>
#include <dali/public-api/images/native-image.h>
#include <dali/devel-api/images/atlas.h>
#include <dali/devel-api/adaptor-framework/bitmap-loader.h>
#include <dali/integration-api/debug.h>

// INTERNAL HEADER
#include <dali-toolkit/internal/controls/renderers/renderer-string-constants.h>
#include <dali-toolkit/internal/controls/renderers/renderer-factory-impl.h>
#include <dali-toolkit/internal/controls/renderers/renderer-factory-cache.h>
#include <dali-toolkit/internal/controls/renderers/control-renderer-impl.h>
#include <dali-toolkit/internal/controls/renderers/control-renderer-data-impl.h>
#include <dali-toolkit/internal/controls/renderers/image-atlas-manager.h>

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

namespace
{
const char HTTP_URL[] = "http://";
const char HTTPS_URL[] = "https://";

// property names
const char * const IMAGE_FITTING_MODE( "fittingMode" );
const char * const IMAGE_SAMPLING_MODE( "samplingMode" );
const char * const IMAGE_DESIRED_WIDTH( "desiredWidth" );
const char * const IMAGE_DESIRED_HEIGHT( "desiredHeight" );
const char * const SYNCHRONOUS_LOADING( "synchronousLoading" );

// fitting modes
const char * const SHRINK_TO_FIT("SHRINK_TO_FIT");
const char * const SCALE_TO_FILL("SCALE_TO_FILL");
const char * const FIT_WIDTH("FIT_WIDTH");
const char * const FIT_HEIGHT("FIT_HEIGHT");
const char * const DEFAULT("DEFAULT");

// sampling modes
const char * const BOX("BOX");
const char * const NEAREST("NEAREST");
const char * const LINEAR("LINEAR");
const char * const BOX_THEN_NEAREST("BOX_THEN_NEAREST");
const char * const BOX_THEN_LINEAR("BOX_THEN_LINEAR");
const char * const NO_FILTER("NO_FILTER");
const char * const DONT_CARE("DONT_CARE");

const std::string PIXEL_AREA_UNIFORM_NAME = "pixelArea";

const Vector4 FULL_TEXTURE_RECT(0.f, 0.f, 1.f, 1.f);

const char* DEFAULT_SAMPLER_TYPENAME = "sampler2D";

const char* VERTEX_SHADER = DALI_COMPOSE_SHADER(
  attribute mediump vec2 aPosition;\n
  uniform mediump mat4 uMvpMatrix;\n
  uniform mediump vec3 uSize;\n
  uniform mediump vec4 uAtlasRect;\n
  uniform mediump vec4 pixelArea;
  varying mediump vec2 vTexCoord;\n
  \n
  void main()\n
  {\n
    mediump vec4 vertexPosition = vec4(aPosition, 0.0, 1.0);\n
    vertexPosition.xyz *= uSize;\n
    vertexPosition = uMvpMatrix * vertexPosition;\n
    \n
    vTexCoord = mix( uAtlasRect.xy, uAtlasRect.zw, pixelArea.xy+pixelArea.zw*(aPosition + vec2(0.5) ) );\n
    gl_Position = vertexPosition;\n
  }\n
);

const char* FRAGMENT_SHADER = DALI_COMPOSE_SHADER(
  varying mediump vec2 vTexCoord;\n
  uniform sampler2D sTexture;\n
  uniform lowp vec4 uColor;\n
  \n
  void main()\n
  {\n
    gl_FragColor = texture2D( sTexture, vTexCoord ) * uColor;\n
  }\n
);

Geometry CreateGeometry( RendererFactoryCache& factoryCache, ImageDimensions gridSize )
{
  Geometry geometry;

  if( gridSize == ImageDimensions( 1, 1 ) )
  {
    geometry = factoryCache.GetGeometry( RendererFactoryCache::QUAD_GEOMETRY );
    if( !geometry )
    {
      geometry =  Geometry::QUAD();
      factoryCache.SaveGeometry( RendererFactoryCache::QUAD_GEOMETRY, geometry );
    }
  }
  else
  {
    geometry = RendererFactoryCache::CreateGridGeometry( gridSize );
  }

  return geometry;
}

} //unnamed namespace

ImageRenderer::ImageRenderer( RendererFactoryCache& factoryCache, ImageAtlasManager& atlasManager )
: ControlRenderer( factoryCache ),
  mAtlasManager( atlasManager ),
  mDesiredSize(),
  mFittingMode( FittingMode::DEFAULT ),
  mSamplingMode( SamplingMode::DEFAULT ),
  mNativeFragmentShaderCode( ),
  mNativeImageFlag( false )
{
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::DoInitialize( Actor& actor, const Property::Map& propertyMap )
{
  std::string oldImageUrl = mImageUrl;

  Property::Value* imageURLValue = propertyMap.Find( IMAGE_URL_NAME );
  if( imageURLValue )
  {
    imageURLValue->Get( mImageUrl );
    if( !mImageUrl.empty() )
    {
      mImage.Reset();
    }

    Property::Value* fittingValue = propertyMap.Find( IMAGE_FITTING_MODE );
    if( fittingValue )
    {
      std::string fitting;
      fittingValue->Get( fitting );

      mFittingMode = FittingMode::DEFAULT;
      if( fitting == SHRINK_TO_FIT )
      {
        mFittingMode = FittingMode::SHRINK_TO_FIT;
      }
      else if( fitting == SCALE_TO_FILL )
      {
        mFittingMode = FittingMode::SCALE_TO_FILL;
      }
      else if( fitting == FIT_WIDTH )
      {
        mFittingMode = FittingMode::FIT_WIDTH;
      }
      else if( fitting == FIT_HEIGHT )
      {
        mFittingMode = FittingMode::FIT_HEIGHT;
      }
      else if( fitting == DEFAULT )
      {
        mFittingMode = FittingMode::DEFAULT;
      }
      else
      {
        DALI_ASSERT_ALWAYS("Unknown fitting mode");
      }
    }

    Property::Value* samplingValue = propertyMap.Find( IMAGE_SAMPLING_MODE );
    if( samplingValue )
    {
      std::string sampling;
      samplingValue->Get( sampling );

      mSamplingMode = SamplingMode::DEFAULT;
      if( sampling == BOX )
      {
        mSamplingMode = SamplingMode::BOX;
      }
      else if( sampling == NEAREST )
      {
        mSamplingMode = SamplingMode::NEAREST;
      }
      else if( sampling == LINEAR )
      {
        mSamplingMode = SamplingMode::LINEAR;
      }
      else if( sampling == BOX_THEN_NEAREST )
      {
        mSamplingMode = SamplingMode::BOX_THEN_NEAREST;
      }
      else if( sampling == BOX_THEN_LINEAR )
      {
        mSamplingMode = SamplingMode::BOX_THEN_LINEAR;
      }
      else if( sampling == NO_FILTER )
      {
        mSamplingMode = SamplingMode::NO_FILTER;
      }
      else if( sampling == DONT_CARE )
      {
        mSamplingMode = SamplingMode::DONT_CARE;
      }
      else if( sampling == DEFAULT )
      {
        mSamplingMode = SamplingMode::DEFAULT;
      }
      else
      {
        DALI_ASSERT_ALWAYS("Unknown sampling mode");
      }
    }

    int desiredWidth = 0;
    Property::Value* desiredWidthValue = propertyMap.Find( IMAGE_DESIRED_WIDTH );
    if( desiredWidthValue )
    {
      desiredWidthValue->Get( desiredWidth );
    }

    int desiredHeight = 0;
    Property::Value* desiredHeightValue = propertyMap.Find( IMAGE_DESIRED_HEIGHT );
    if( desiredHeightValue )
    {
      desiredHeightValue->Get( desiredHeight );
    }

    mDesiredSize = ImageDimensions( desiredWidth, desiredHeight );

  }

  Property::Value* syncLoading = propertyMap.Find( SYNCHRONOUS_LOADING );
  if( syncLoading )
  {
    bool sync;
    syncLoading->Get( sync );
    if( sync )
    {
      mImpl->mFlags |= Impl::IS_SYNCHRONOUS_RESOURCE_LOADING;
    }
    else
    {
      mImpl->mFlags &= ~Impl::IS_SYNCHRONOUS_RESOURCE_LOADING;
    }
  }

  // if sync loading is required, the loading should start immediately when new image url is set or the actor is off stage
  // ( for on-stage actor with image url unchanged, resource loading is already finished)
  if( ( !mImpl->mRenderer || imageURLValue) && IsSynchronousResourceLoading() )
  {
    DoSynchronousResourceLoading();
  }

  // remove old renderer if exit
  if( mImpl->mRenderer )
  {
    if( actor ) //remove old renderer from actor
    {
      actor.RemoveRenderer( mImpl->mRenderer );
    }
    if( !oldImageUrl.empty() ) //clean old renderer from cache
    {
      CleanCache( oldImageUrl );
    }
  }

  NativeImage nativeImage = NativeImage::DownCast( mImage );

  if( nativeImage )
  {
    SetNativeFragmentShaderCode( nativeImage );
  }

  // if actor is on stage, create new renderer and apply to actor
  if( actor && actor.OnStage() )
  {
    SetOnStage( actor );
  }
}

void ImageRenderer::SetSize( const Vector2& size )
{
  ControlRenderer::SetSize( size );
}

void ImageRenderer::GetNaturalSize( Vector2& naturalSize ) const
{
  if(mImage)
  {
    naturalSize.x = mImage.GetWidth();
    naturalSize.y = mImage.GetHeight();
    return;
  }
  else if( mDesiredSize.GetWidth()>0 && mDesiredSize.GetHeight()>0)
  {
    naturalSize.x = mDesiredSize.GetWidth();
    naturalSize.y = mDesiredSize.GetHeight();
    return;
  }
  else if( !mImageUrl.empty() )
  {
    ImageDimensions dimentions = ResourceImage::GetImageSize( mImageUrl );
    naturalSize.x = dimentions.GetWidth();
    naturalSize.y = dimentions.GetHeight();
    return;
  }

  naturalSize = Vector2::ZERO;
}

void ImageRenderer::SetClipRect( const Rect<int>& clipRect )
{
  ControlRenderer::SetClipRect( clipRect );
}

void ImageRenderer::SetOffset( const Vector2& offset )
{
}

Renderer ImageRenderer::CreateRenderer() const
{
  Geometry geometry;
  Shader shader;

  // If mImage is nativeImage with custom sampler or prefix, mNativeFragmentShaderCode will be applied.
  // Renderer can't be shared between NativeImage and other image types.
  if( !mNativeFragmentShaderCode.empty() )
  {
    return CreateNativeImageRenderer();
  }

  if( !mImpl->mCustomShader )
  {
    geometry = CreateGeometry( mFactoryCache, ImageDimensions( 1, 1 ) );

    shader = GetImageShader(mFactoryCache);
  }
  else
  {
    geometry = CreateGeometry( mFactoryCache, mImpl->mCustomShader->mGridSize );
    if( mImpl->mCustomShader->mVertexShader.empty() && mImpl->mCustomShader->mFragmentShader.empty() )
    {
      shader = GetImageShader(mFactoryCache);
    }
    else
    {
      shader  = Shader::New( mImpl->mCustomShader->mVertexShader.empty() ? VERTEX_SHADER : mImpl->mCustomShader->mVertexShader,
                             mImpl->mCustomShader->mFragmentShader.empty() ? FRAGMENT_SHADER : mImpl->mCustomShader->mFragmentShader,
                             mImpl->mCustomShader->mHints );
      if( mImpl->mCustomShader->mVertexShader.empty() )
      {
        shader.RegisterProperty( ATLAS_RECT_UNIFORM_NAME, FULL_TEXTURE_RECT );
        shader.RegisterProperty( PIXEL_AREA_UNIFORM_NAME, FULL_TEXTURE_RECT );
      }
    }
  }

  Renderer renderer = Renderer::New( geometry, shader );

  return renderer;
}

Renderer ImageRenderer::CreateNativeImageRenderer() const
{
  Geometry geometry;
  Shader shader;

  if( !mImpl->mCustomShader )
  {
    geometry = CreateGeometry( mFactoryCache, ImageDimensions( 1, 1 ) );

    shader  = Shader::New( VERTEX_SHADER, mNativeFragmentShaderCode );
    shader.RegisterProperty( ATLAS_RECT_UNIFORM_NAME, FULL_TEXTURE_RECT );
    shader.RegisterProperty( PIXEL_AREA_UNIFORM_NAME, FULL_TEXTURE_RECT );
  }
  else
  {
    geometry = CreateGeometry( mFactoryCache, mImpl->mCustomShader->mGridSize );
    shader  = Shader::New( mImpl->mCustomShader->mVertexShader.empty() ? VERTEX_SHADER : mImpl->mCustomShader->mVertexShader,
                           mNativeFragmentShaderCode,
                           mImpl->mCustomShader->mHints );
    if( mImpl->mCustomShader->mVertexShader.empty() )
    {
      shader.RegisterProperty( ATLAS_RECT_UNIFORM_NAME, FULL_TEXTURE_RECT );
      shader.RegisterProperty( PIXEL_AREA_UNIFORM_NAME, FULL_TEXTURE_RECT );
    }
  }

  TextureSet textureSet = TextureSet::New();
  Renderer renderer = Renderer::New( geometry, shader );
  renderer.SetTextures( textureSet );

  return renderer;
}


bool ImageRenderer::IsSynchronousResourceLoading() const
{
  return mImpl->mFlags & Impl::IS_SYNCHRONOUS_RESOURCE_LOADING;
}

void ImageRenderer::DoSynchronousResourceLoading()
{
  if( !mImageUrl.empty() )
  {
    BitmapLoader loader = BitmapLoader::New( mImageUrl, mDesiredSize, mFittingMode, mSamplingMode );
    loader.Load();
    mPixels = loader.GetPixelData();
  }
}

Image ImageRenderer::LoadImage( const std::string& url, bool synchronousLoading )
{
  if( synchronousLoading )
  {
    if( !mPixels )
    {
      // use broken image
      return RendererFactory::GetBrokenRendererImage();
    }
    Atlas image = Atlas::New( mPixels->GetWidth(), mPixels->GetHeight(), mPixels->GetPixelFormat() );
    image.Upload( mPixels, 0, 0 );
    return image;
  }
  else
  {
    ResourceImage resourceImage = Dali::ResourceImage::New( url, mDesiredSize, mFittingMode, mSamplingMode );
    resourceImage.LoadingFinishedSignal().Connect( this, &ImageRenderer::OnImageLoaded );
    return resourceImage;
  }
}

TextureSet ImageRenderer::CreateTextureSet( Vector4& textureRect, const std::string& url, bool synchronousLoading )
{
  TextureSet textureSet;
  textureRect = FULL_TEXTURE_RECT;
  if( synchronousLoading )
  {
    if( !mPixels )
    {
      // use broken image
      textureSet = TextureSet::New();
      textureSet.SetImage( 0u, RendererFactory::GetBrokenRendererImage() );
    }
    else
    {
      textureSet = mAtlasManager.Add(textureRect, mPixels );
      if( !textureSet ) // big image, no atlasing
      {
        Atlas image = Atlas::New( mPixels->GetWidth(), mPixels->GetHeight(), mPixels->GetPixelFormat() );
        image.Upload( mPixels, 0, 0 );
        textureSet = TextureSet::New();
        textureSet.SetImage( 0u, image );
      }
    }
  }
  else
  {
    textureSet = mAtlasManager.Add(textureRect, url, mDesiredSize, mFittingMode, mSamplingMode );
    if( !textureSet ) // big image, no atlasing
    {
      ResourceImage resourceImage = Dali::ResourceImage::New( url, mDesiredSize, mFittingMode, mSamplingMode );
      resourceImage.LoadingFinishedSignal().Connect( this, &ImageRenderer::OnImageLoaded );
      textureSet = TextureSet::New();
      textureSet.SetImage( 0u, resourceImage );
    }
  }

  return textureSet;
}

void ImageRenderer::InitializeRenderer( const std::string& imageUrl )
{
  if( imageUrl.empty() )
  {
    return;
  }

  mImageUrl = imageUrl;
  mImpl->mRenderer.Reset();

  if( !mImpl->mCustomShader &&
      ( strncasecmp( imageUrl.c_str(), HTTP_URL,  sizeof(HTTP_URL)  -1 ) != 0 ) && // ignore remote images
      ( strncasecmp( imageUrl.c_str(), HTTPS_URL, sizeof(HTTPS_URL) -1 ) != 0 ) )
  {
    mImpl->mRenderer = mFactoryCache.GetRenderer( imageUrl );
    if( !mImpl->mRenderer )
    {
      Vector4 atlasRect;
      TextureSet textureSet = CreateTextureSet(atlasRect, imageUrl, IsSynchronousResourceLoading() );
      Geometry geometry = CreateGeometry( mFactoryCache, ImageDimensions( 1, 1 ) );
      Shader shader( GetImageShader(mFactoryCache) );
      mImpl->mRenderer = Renderer::New( geometry, shader );
      mImpl->mRenderer.SetTextures( textureSet );
      if( atlasRect != FULL_TEXTURE_RECT )
      {
        mImpl->mRenderer.RegisterProperty( ATLAS_RECT_UNIFORM_NAME, atlasRect );
      }
      mFactoryCache.SaveRenderer( imageUrl, mImpl->mRenderer );
    }

    mImpl->mFlags |= Impl::IS_FROM_CACHE;
  }
  else
  {
    // for custom shader or remote image, renderer is not cached and atlas is not applied

    mImpl->mFlags &= ~Impl::IS_FROM_CACHE;
    mImpl->mRenderer = CreateRenderer();
    Image image = LoadImage( imageUrl, IsSynchronousResourceLoading() );
    ApplyImageToSampler( image );
  }
}

void ImageRenderer::InitializeRenderer( const Image& image )
{
  mImpl->mFlags &= ~Impl::IS_FROM_CACHE;

  mImpl->mRenderer = CreateRenderer();

  if( image )
  {
    ApplyImageToSampler( image );
  }
}


void ImageRenderer::DoSetOnStage( Actor& actor )
{
  if( !mImageUrl.empty() )
  {
    InitializeRenderer( mImageUrl );
  }
  else
  {
    InitializeRenderer( mImage );
  }

}

void ImageRenderer::DoSetOffStage( Actor& actor )
{
  //If we own the image then make sure we release it when we go off stage
  if( !mImageUrl.empty() )
  {
    actor.RemoveRenderer( mImpl->mRenderer );
    CleanCache(mImageUrl);
    mImage.Reset();
  }
  else
  {
    actor.RemoveRenderer( mImpl->mRenderer );
    mImpl->mRenderer.Reset();
  }
}

void ImageRenderer::DoCreatePropertyMap( Property::Map& map ) const
{
  map.Clear();
  map.Insert( RENDERER_TYPE, IMAGE_RENDERER );

  bool sync = IsSynchronousResourceLoading();
  map.Insert( SYNCHRONOUS_LOADING, sync );
  if( !mImageUrl.empty() )
  {
    map.Insert( IMAGE_URL_NAME, mImageUrl );
    map.Insert( IMAGE_DESIRED_WIDTH, mDesiredSize.GetWidth() );
    map.Insert( IMAGE_DESIRED_HEIGHT, mDesiredSize.GetHeight() );
  }
  else if( mImage )
  {
    map.Insert( IMAGE_DESIRED_WIDTH, static_cast<int>(mImage.GetWidth()) );
    map.Insert( IMAGE_DESIRED_HEIGHT, static_cast<int>(mImage.GetHeight()) );

    ResourceImage resourceImage = ResourceImage::DownCast(mImage);
    if( resourceImage )
    {
      map.Insert( IMAGE_URL_NAME, resourceImage.GetUrl() );
    }
  }

  switch( mFittingMode )
  {
    case Dali::FittingMode::FIT_HEIGHT:
    {
      map.Insert( IMAGE_FITTING_MODE, FIT_HEIGHT );
      break;
    }
    case Dali::FittingMode::FIT_WIDTH:
    {
      map.Insert( IMAGE_FITTING_MODE, FIT_WIDTH );
      break;
    }
    case Dali::FittingMode::SCALE_TO_FILL:
    {
      map.Insert( IMAGE_FITTING_MODE, SCALE_TO_FILL );
      break;
    }
    case Dali::FittingMode::SHRINK_TO_FIT:
    {
      map.Insert( IMAGE_FITTING_MODE, SHRINK_TO_FIT );
      break;
    }
    default:
    {
      map.Insert( IMAGE_FITTING_MODE, DEFAULT );
      break;
    }
  }

  switch( mSamplingMode )
  {
    case Dali::SamplingMode::BOX:
    {
      map.Insert( IMAGE_SAMPLING_MODE, BOX );
      break;
    }
    case Dali::SamplingMode::NEAREST:
    {
      map.Insert( IMAGE_SAMPLING_MODE, NEAREST );
      break;
    }
    case Dali::SamplingMode::LINEAR:
    {
      map.Insert( IMAGE_SAMPLING_MODE, LINEAR );
      break;
    }
    case Dali::SamplingMode::BOX_THEN_LINEAR:
    {
      map.Insert( IMAGE_SAMPLING_MODE, BOX_THEN_LINEAR );
      break;
    }
    case Dali::SamplingMode::BOX_THEN_NEAREST:
    {
      map.Insert( IMAGE_SAMPLING_MODE, BOX_THEN_NEAREST );
      break;
    }
    case Dali::SamplingMode::NO_FILTER:
    {
      map.Insert( IMAGE_SAMPLING_MODE, NO_FILTER );
      break;
    }
    case Dali::SamplingMode::DONT_CARE:
    {
      map.Insert( IMAGE_SAMPLING_MODE, DONT_CARE );
      break;
    }
    default:
    {
      map.Insert( IMAGE_SAMPLING_MODE, DEFAULT );
      break;
    }
  }
}

Shader ImageRenderer::GetImageShader( RendererFactoryCache& factoryCache )
{
  Shader shader = factoryCache.GetShader( RendererFactoryCache::IMAGE_SHADER );
  if( !shader )
  {
    shader = Shader::New( VERTEX_SHADER, FRAGMENT_SHADER );
    factoryCache.SaveShader( RendererFactoryCache::IMAGE_SHADER, shader );
    shader.RegisterProperty( ATLAS_RECT_UNIFORM_NAME, FULL_TEXTURE_RECT );
    shader.RegisterProperty( PIXEL_AREA_UNIFORM_NAME, FULL_TEXTURE_RECT );
  }
  return shader;
}

void ImageRenderer::SetImage( Actor& actor, const std::string& imageUrl, ImageDimensions size, Dali::FittingMode::Type fittingMode, Dali::SamplingMode::Type samplingMode )
{
  if( mImageUrl != imageUrl )
  {
    std::string oldImageUrl = mImageUrl;
    mImageUrl = imageUrl;
    mDesiredSize = size;
    mFittingMode = fittingMode;
    mSamplingMode = samplingMode;
    mImage.Reset();

    if( IsSynchronousResourceLoading() )
    {
      DoSynchronousResourceLoading();
    }

    if( mImpl->mRenderer )
    {
      if( GetIsFromCache() ) // if renderer is from cache, remove the old one
      {
        //remove old renderer
        if( actor )
        {
          actor.RemoveRenderer( mImpl->mRenderer );
        }

        //clean the cache
        if( !oldImageUrl.empty() )
        {
          CleanCache(oldImageUrl);
        }

        if( actor && actor.OnStage() ) // if actor on stage, create a new renderer and apply to actor
        {
          SetOnStage(actor);
        }
      }
      else // if renderer is not from cache, reuse the same renderer and only change the texture
      {
        Image image = LoadImage( imageUrl, IsSynchronousResourceLoading() );
        ApplyImageToSampler( image );
      }
    }
  }
}

void ImageRenderer::SetImage( Actor& actor, const Image& image )
{
  if( mImage != image )
  {
    NativeImage newNativeImage = NativeImage::DownCast( image );
    bool newRendererFlag = true;

    if( newNativeImage && !mNativeImageFlag )
    {
      SetNativeFragmentShaderCode( newNativeImage );
    }

    if( ( newNativeImage && mNativeImageFlag ) || ( !newNativeImage && !mNativeImageFlag ) )
    {
      newRendererFlag = false;
    }

    if( newNativeImage )
    {
      mNativeImageFlag = true;
    }
    else
    {
      mNativeFragmentShaderCode.clear();
      mNativeImageFlag = false;
    }

    mImage = image;

    if( mImpl->mRenderer )
    {
      // if renderer is from cache, remove the old one, and create new renderer
      if( GetIsFromCache() )
      {
        //remove old renderer
        if( actor )
        {
          actor.RemoveRenderer( mImpl->mRenderer );
        }

        //clean the cache
        if( !mImageUrl.empty() )
        {
          CleanCache(mImageUrl);
        }
        mImageUrl.clear();

        if( actor && actor.OnStage() ) // if actor on stage, create a new renderer and apply to actor
        {
          SetOnStage(actor);
        }
      }
      // if input image is nativeImage and mImage is regular image or the reverse, remove the old one, and create new renderer
      else if( newRendererFlag )
      {
        //remove old renderer
        if( actor )
        {
          actor.RemoveRenderer( mImpl->mRenderer );
        }

        if( actor && actor.OnStage() ) // if actor on stage, create a new renderer and apply to actor
        {
          SetOnStage(actor);
        }
      }
      else // if renderer is not from cache, reuse the same renderer and only change the texture
      {
        ApplyImageToSampler( image );
      }
    }

    mImageUrl.clear();
    mDesiredSize = ImageDimensions();
    mFittingMode = FittingMode::DEFAULT;
    mSamplingMode = SamplingMode::DEFAULT;
  }
}

void ImageRenderer::ApplyImageToSampler( const Image& image )
{
  if( image )
  {
    TextureSet textureSet = mImpl->mRenderer.GetTextures();
    if( !textureSet )
    {
      textureSet = TextureSet::New();
      mImpl->mRenderer.SetTextures( textureSet );
    }
    textureSet.SetImage( 0u, image );
  }
}

void ImageRenderer::OnImageLoaded( ResourceImage image )
{
  if( image.GetLoadingState() == Dali::ResourceLoadingFailed )
  {
    Image brokenImage = RendererFactory::GetBrokenRendererImage();
    if( mImpl->mRenderer )
    {
      ApplyImageToSampler( brokenImage );
    }
  }
}

void ImageRenderer::CleanCache(const std::string& url)
{
  TextureSet textureSet = mImpl->mRenderer.GetTextures();

  Vector4 atlasRect( 0.f, 0.f, 1.f, 1.f );
  Property::Index index = mImpl->mRenderer.GetPropertyIndex( ATLAS_RECT_UNIFORM_NAME );
  if( index != Property::INVALID_INDEX )
  {
    Property::Value atlasRectValue = mImpl->mRenderer.GetProperty( index );
    atlasRectValue.Get( atlasRect );
  }

  mImpl->mRenderer.Reset();
  if( mFactoryCache.CleanRendererCache( url ) && index != Property::INVALID_INDEX )
  {
    mAtlasManager.Remove( textureSet, atlasRect );
  }
}

void ImageRenderer::SetNativeFragmentShaderCode( Dali::NativeImage& nativeImage )
{
  const char* fragmentPreFix = nativeImage.GetCustomFragmentPreFix();
  const char* customSamplerTypename = nativeImage.GetCustomSamplerTypename();

  if( fragmentPreFix )
  {
    mNativeFragmentShaderCode = fragmentPreFix;
    mNativeFragmentShaderCode += "\n";
  }

  if( mImpl->mCustomShader && !mImpl->mCustomShader->mFragmentShader.empty() )
  {
    mNativeFragmentShaderCode += mImpl->mCustomShader->mFragmentShader;
  }
  else
  {
    mNativeFragmentShaderCode += FRAGMENT_SHADER;
  }

  if( customSamplerTypename )
  {
    mNativeFragmentShaderCode.replace( mNativeFragmentShaderCode.find( DEFAULT_SAMPLER_TYPENAME ), strlen( DEFAULT_SAMPLER_TYPENAME ), customSamplerTypename );
  }
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
