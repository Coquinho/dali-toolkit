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
#include "visual-base-impl.h"

// EXTERNAL HEADER
#include <dali/public-api/common/dali-common.h>
#include <dali/integration-api/debug.h>

//INTERNAL HEARDER
#include <dali-toolkit/public-api/visuals/visual-properties.h>
#include <dali-toolkit/internal/visuals/visual-base-data-impl.h>
#include <dali-toolkit/internal/visuals/visual-string-constants.h>

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

namespace Visual
{

Base::Base( VisualFactoryCache& factoryCache )
: mImpl( new Impl() ),
  mFactoryCache( factoryCache )
{
}

Base::~Base()
{
  delete mImpl;
}

void Base::SetCustomShader( const Property::Map& shaderMap )
{
  if( mImpl->mCustomShader )
  {
    mImpl->mCustomShader->SetPropertyMap( shaderMap );
  }
  else
  {
   mImpl->mCustomShader = new Impl::CustomShader( shaderMap );
  }
}

void Base::Initialize( Actor& actor, const Property::Map& propertyMap )
{
  Property::Value* customShaderValue = propertyMap.Find( Toolkit::Visual::Property::SHADER, CUSTOM_SHADER );
  if( customShaderValue )
  {
    Property::Map shaderMap;
    if( customShaderValue->Get( shaderMap ) )
    {
      SetCustomShader( shaderMap );
    }
  }

  DoInitialize( actor, propertyMap );
}

void Base::SetSize( const Vector2& size )
{
  mImpl->mSize = size;
}

const Vector2& Base::GetSize() const
{
  return mImpl->mSize;
}

void Base::GetNaturalSize( Vector2& naturalSize ) const
{
  naturalSize = Vector2::ZERO;
}

void Base::SetClipRect( const Rect<int>& clipRect )
{
}

void Base::SetOffset( const Vector2& offset )
{
  mImpl->mOffset = offset;
}

void Base::SetDepthIndex( float index )
{
  mImpl->mDepthIndex = index;
  if( mImpl->mRenderer )
  {
    mImpl->mRenderer.SetProperty( Renderer::Property::DEPTH_INDEX, mImpl->mDepthIndex );
  }
}

float Base::GetDepthIndex() const
{
  return mImpl->mDepthIndex;
}

void Base::SetOnStage( Actor& actor )
{
  DoSetOnStage( actor );

  mImpl->mRenderer.SetProperty(Renderer::Property::BLEND_PRE_MULTIPLIED_ALPHA, IsPreMultipliedAlphaEnabled());
  mImpl->mRenderer.SetProperty( Renderer::Property::DEPTH_INDEX, mImpl->mDepthIndex );
  actor.AddRenderer( mImpl->mRenderer );

  mImpl->mFlags |= Impl::IS_ON_STAGE;
}

void Base::SetOffStage( Actor& actor )
{
  if( GetIsOnStage() )
  {
    DoSetOffStage( actor );

    mImpl->mFlags &= ~Impl::IS_ON_STAGE;
  }
}

void Base::EnablePreMultipliedAlpha( bool preMultipled )
{
  if(preMultipled)
  {
    mImpl->mFlags |= Impl::IS_PREMULTIPLIED_ALPHA;
  }
  else
  {
    mImpl->mFlags &= ~Impl::IS_PREMULTIPLIED_ALPHA;
  }

  if( mImpl->mRenderer )
  {
    mImpl->mRenderer.SetProperty(Renderer::Property::BLEND_PRE_MULTIPLIED_ALPHA, preMultipled);
  }
}

bool Base::IsPreMultipliedAlphaEnabled() const
{
  return mImpl->mFlags & Impl::IS_PREMULTIPLIED_ALPHA;
}

void Base::DoSetOnStage( Actor& actor )
{
}

void Base::DoSetOffStage( Actor& actor )
{
  actor.RemoveRenderer( mImpl->mRenderer );
  mImpl->mRenderer.Reset();
}

void Base::CreatePropertyMap( Property::Map& map ) const
{
  DoCreatePropertyMap( map );

  if( mImpl->mCustomShader )
  {
    mImpl->mCustomShader->CreatePropertyMap( map );
  }
}

bool Base::GetIsOnStage() const
{
  return mImpl->mFlags & Impl::IS_ON_STAGE;
}

bool Base::GetIsFromCache() const
{
  return mImpl->mFlags & Impl::IS_FROM_CACHE;
}

} // namespace Visual

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
