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
 */

// CLASS HEADER
#include <dali-toolkit/internal/atlas-manager/atlas-manager.h>

 // INTERNAL INCLUDES
#include <dali-toolkit/internal/atlas-manager/atlas-manager-impl.h>

namespace Dali
{

namespace Toolkit
{

AtlasManager::AtlasManager()
{
}

AtlasManager::~AtlasManager()
{
}

AtlasManager AtlasManager::New()
{
  Internal::AtlasManagerPtr internal = Internal::AtlasManager::New();
  return AtlasManager(internal.Get());
}

AtlasManager::AtlasManager(Internal::AtlasManager *impl)
  : BaseHandle(impl)
{
}

AtlasManager::AtlasId AtlasManager::CreateAtlas( const AtlasManager::AtlasSize& size, Pixel::Format pixelformat )
{
  return GetImplementation(*this).CreateAtlas( size, pixelformat );
}

void AtlasManager::SetAddPolicy( AddFailPolicy policy )
{
  GetImplementation(*this).SetAddPolicy( policy );
}

void AtlasManager::Add( const BufferImage& image,
                        AtlasManager::AtlasSlot& slot,
                        AtlasManager::AtlasId atlas )
{
  GetImplementation(*this).Add( image, slot, atlas );
}

bool AtlasManager::Remove( ImageId id )
{
  return GetImplementation(*this).Remove( id );
}

void AtlasManager::GenerateMeshData( ImageId id,
                                     const Vector2& position,
                                     Mesh2D& mesh,
                                     bool addReference )
{
  GetImplementation(*this).GenerateMeshData( id,
                                             position,
                                             mesh,
                                             addReference );
}

void AtlasManager::StitchMesh( Mesh2D& first,
                               const Mesh2D& second,
                               bool optimize )
{
  GetImplementation(*this).StitchMesh( first, second, optimize );
}

void AtlasManager::StitchMesh( const Mesh2D& first,
                               const Mesh2D& second,
                               Mesh2D& out,
                               bool optimize )
{
  GetImplementation(*this).StitchMesh( first, second, out, optimize );
}

Dali::Atlas AtlasManager::GetAtlasContainer( AtlasId atlas ) const
{
  return GetImplementation(*this).GetAtlasContainer( atlas );
}

AtlasManager::AtlasId AtlasManager::GetAtlas( ImageId id )
{
  return GetImplementation(*this).GetAtlas( id );
}

const AtlasManager::AtlasSize& AtlasManager::GetAtlasSize( AtlasId atlas )
{
  return GetImplementation(*this).GetAtlasSize( atlas );
}

AtlasManager::SizeType AtlasManager::GetFreeBlocks( AtlasId atlas )
{
  return GetImplementation(*this).GetFreeBlocks( atlas );
}

void AtlasManager::SetNewAtlasSize( const AtlasSize& size )
{
  GetImplementation(*this).SetNewAtlasSize( size );
}

AtlasManager::SizeType AtlasManager::GetAtlasCount() const
{
  return GetImplementation(*this).GetAtlasCount();
}

Pixel::Format AtlasManager::GetPixelFormat( AtlasId atlas )
{
  return GetImplementation(*this).GetPixelFormat( atlas );
}

void AtlasManager::GetMetrics( Metrics& metrics )
{
  GetImplementation(*this).GetMetrics( metrics );
}

Material AtlasManager::GetMaterial( AtlasId atlas ) const
{
  return GetImplementation(*this).GetMaterial( atlas );
}

Sampler AtlasManager::GetSampler( AtlasId atlas ) const
{
  return GetImplementation(*this).GetSampler( atlas );
}

} // namespace Toolkit

} // namespace Dali
