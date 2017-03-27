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

#include <iostream>
#include <stdlib.h>
#include <unistd.h>

#include <dali-toolkit-test-suite-utils.h>
#include <dali/public-api/rendering/renderer.h>
#include <dali/public-api/rendering/texture-set.h>
#include <dali/public-api/rendering/shader.h>
#include <dali/devel-api/object/handle-devel.h>
#include <dali-toolkit/devel-api/visual-factory/visual-factory.h>
#include <dali-toolkit/devel-api/visual-factory/transition-data.h>
#include <dali-toolkit/devel-api/visuals/text-visual-properties.h>
#include <dali-toolkit/devel-api/visuals/visual-properties-devel.h>
#include <dali-toolkit/devel-api/visuals/image-visual-properties-devel.h>
#include <dali-toolkit/devel-api/align-enums.h>
#include <dali-toolkit/dali-toolkit.h>
#include "dummy-control.h"

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
const char* TEST_IMAGE_FILE_NAME =  TEST_RESOURCE_DIR "/gallery-small-1.jpg";
const char* TEST_NPATCH_FILE_NAME =  "gallery_image_01.9.jpg";
const char* TEST_SVG_FILE_NAME = TEST_RESOURCE_DIR "/svg1.svg";
const char* TEST_GIF_FILE_NAME = TEST_RESOURCE_DIR "/anim.gif";
const char* TEST_OBJ_FILE_NAME = TEST_RESOURCE_DIR "/Cube.obj";
const char* TEST_MTL_FILE_NAME = TEST_RESOURCE_DIR "/ToyRobot-Metal.mtl";
const char* TEST_RESOURCE_LOCATION = TEST_RESOURCE_DIR "/";

const std::string DEFAULT_FONT_DIR( "/resources/fonts" );

Property::Map DefaultTransform()
{
  Property::Map transformMap;
  transformMap
    .Add( Toolkit::DevelVisual::Transform::Property::OFFSET, Vector2(0.0f, 0.0f) )
    .Add( Toolkit::DevelVisual::Transform::Property::SIZE, Vector2(1.0f, 1.0f) )
    .Add( Toolkit::DevelVisual::Transform::Property::ORIGIN, Toolkit::Align::CENTER )
    .Add( Toolkit::DevelVisual::Transform::Property::ANCHOR_POINT, Toolkit::Align::CENTER )
    .Add( Toolkit::DevelVisual::Transform::Property::OFFSET_POLICY, Vector2( Toolkit::DevelVisual::Transform::Policy::RELATIVE, Toolkit::DevelVisual::Transform::Policy::RELATIVE ) )
    .Add( Toolkit::DevelVisual::Transform::Property::SIZE_POLICY, Vector2( Toolkit::DevelVisual::Transform::Policy::RELATIVE, Toolkit::DevelVisual::Transform::Policy::RELATIVE ) );
  return transformMap;
}

bool DaliTestCheckMaps( const Property::Map& fontStyleMapGet, const Property::Map& fontStyleMapSet )
{
  if( fontStyleMapGet.Count() == fontStyleMapSet.Count() )
  {
    for( unsigned int index = 0u; index < fontStyleMapGet.Count(); ++index )
    {
      const KeyValuePair& valueGet = fontStyleMapGet.GetKeyValue( index );

      Property::Value* valueSet = fontStyleMapSet.Find( valueGet.first.stringKey );
      if( NULL != valueSet )
      {
        if( valueGet.second.Get<std::string>() != valueSet->Get<std::string>() )
        {
          tet_printf( "  Value got : [%s], expected : [%s]", valueGet.second.Get<std::string>().c_str(), valueSet->Get<std::string>().c_str() );
          return false;
        }
      }
      else
      {
        tet_printf( "  The key %s doesn't exist.", valueGet.first.stringKey.c_str() );
        return false;
      }
    }
  }

  return true;
}

void PrepareResourceImage( ToolkitTestApplication& application, unsigned int imageWidth, unsigned int imageHeight, Pixel::Format pixelFormat )
{
  TestPlatformAbstraction& platform = application.GetPlatform();
  platform.SetClosestImageSize(Vector2( imageWidth, imageHeight));

  Integration::Bitmap* bitmap = Integration::Bitmap::New( Integration::Bitmap::BITMAP_2D_PACKED_PIXELS, ResourcePolicy::OWNED_RETAIN );
  Integration::PixelBuffer* pixbuffer = bitmap->GetPackedPixelsProfile()->ReserveBuffer( pixelFormat, imageWidth, imageHeight, imageWidth, imageHeight );
  unsigned int bytesPerPixel = GetBytesPerPixel(  pixelFormat );
  unsigned int initialColor = 0xFF;
  memset( pixbuffer, initialColor, imageHeight*imageWidth*bytesPerPixel);

  Integration::ResourcePointer resourcePtr(bitmap);
  platform.SetSynchronouslyLoadedResource( resourcePtr );
}
} //namespace

void dali_visual_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_visual_cleanup(void)
{
  test_return_value = TET_PASS;
}


static void TestMixColor( Visual::Base visual, Property::Index mixColorIndex, const Vector4& testColor )
{
  Property::Map map;
  visual.CreatePropertyMap(map);
  Property::Value* value = map.Find( mixColorIndex );
  DALI_TEST_CHECK( value );
  Vector3 mixColor1;
  DALI_TEST_CHECK( value->Get( mixColor1 ) );
  DALI_TEST_EQUALS( mixColor1, Vector3(testColor), 0.001, TEST_LOCATION );

  value = map.Find( DevelVisual::Property::MIX_COLOR );
  DALI_TEST_CHECK( value );
  Vector4 mixColor2;
  DALI_TEST_CHECK( value->Get( mixColor2 ) );
  DALI_TEST_EQUALS( mixColor2, testColor, 0.001, TEST_LOCATION );

  value = map.Find( DevelVisual::Property::OPACITY );
  DALI_TEST_CHECK( value );
  float opacity;
  DALI_TEST_CHECK( value->Get( opacity ) );
  DALI_TEST_EQUALS( opacity, testColor.a, 0.001, TEST_LOCATION );
}


int UtcDaliVisualCopyAndAssignment(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualCopyAndAssignment" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  Visual::Base visualCopy( visual );
  DALI_TEST_CHECK(visual == visualCopy);

  Visual::Base emptyVisual;
  Visual::Base emptyVisualCopy( emptyVisual );
  DALI_TEST_CHECK(emptyVisual == emptyVisualCopy);

  Visual::Base visualEquals;
  visualEquals = visual;
  DALI_TEST_CHECK(visual == visualEquals);

  Visual::Base emptyVisualEquals;
  emptyVisualEquals = emptyVisual;
  DALI_TEST_CHECK( emptyVisual == emptyVisualEquals );

  //self assignment
  visual = visual;
  DALI_TEST_CHECK( visual = visualCopy );

  END_TEST;
}

int UtcDaliVisualSetName01(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetName" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  const char* visualName = "backgroundVisual";
  visual.SetName( visualName );

  DALI_TEST_EQUALS( visual.GetName(), visualName, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualSetGetDepthIndex(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetDepthIndex" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  visual.SetDepthIndex( 1.f );

  DummyControl dummyControl = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(dummyControl.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );

  dummyControl.SetSize(200.f, 200.f);
  Stage::GetCurrent().Add( dummyControl );


  int depthIndex = dummyControl.GetRendererAt(0u).GetProperty<int>( Renderer::Property::DEPTH_INDEX );
  DALI_TEST_EQUALS( depthIndex, 1, TEST_LOCATION );
  DALI_TEST_EQUALS( visual.GetDepthIndex(), 1.f, TEST_LOCATION );

  visual.SetDepthIndex( -1.f );
  depthIndex = dummyControl.GetRendererAt(0u).GetProperty<int>( Renderer::Property::DEPTH_INDEX );
  DALI_TEST_EQUALS( depthIndex, -1, TEST_LOCATION );
  DALI_TEST_EQUALS( visual.GetDepthIndex(), -1.f, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualSize(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetNaturalSize" );

  VisualFactory factory = VisualFactory::Get();
  Vector2 controlSize( 20.f, 30.f );
  Vector2 naturalSize;

  // color colorVisual
  Dali::Property::Map map;
  map[ Visual::Property::TYPE ] = Visual::COLOR;
  map[ ColorVisual::Property::MIX_COLOR ] = Color::MAGENTA;

  Visual::Base colorVisual = factory.CreateVisual( map );
  colorVisual.SetTransformAndSize(DefaultTransform(), controlSize );

  colorVisual.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2::ZERO, TEST_LOCATION );

  // image visual
  PrepareResourceImage( application, 100u, 200u, Pixel::RGBA8888 );
  Image image = ResourceImage::New(TEST_IMAGE_FILE_NAME, ImageDimensions(100, 200));
  Visual::Base imageVisual = factory.CreateVisual( image );
  imageVisual.SetTransformAndSize(DefaultTransform(), controlSize );

  imageVisual.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2(100.f, 200.f), TEST_LOCATION );

  // n patch visual is tested in the utc-Dali-VisualFactory.cpp

  // border visual
  float borderSize = 5.f;
  map.Clear();
  map[ Visual::Property::TYPE ] = Visual::BORDER;
  map[ BorderVisual::Property::COLOR  ] = Color::RED;
  map[ BorderVisual::Property::SIZE   ] = borderSize;
  Visual::Base borderVisual = factory.CreateVisual( map );
  borderVisual.SetTransformAndSize(DefaultTransform(), controlSize );
  borderVisual.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2::ZERO, TEST_LOCATION );

  // gradient gradientVisual
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::GRADIENT);
  Vector2 start(-1.f, -1.f);
  Vector2 end(1.f, 1.f);
  propertyMap.Insert( "mixColor", Color::MAGENTA );
  propertyMap.Insert( GradientVisual::Property::START_POSITION,   start) ;
  propertyMap.Insert( GradientVisual::Property::END_POSITION,   end );
  propertyMap.Insert( GradientVisual::Property::STOP_OFFSET,   Vector2(0.f, 1.f) );
  propertyMap.Insert( GradientVisual::Property::SPREAD_METHOD, GradientVisual::SpreadMethod::REPEAT) ;
  Property::Array stopColors;
  stopColors.PushBack( Color::RED );
  stopColors.PushBack( Color::GREEN );
  propertyMap.Insert(GradientVisual::Property::STOP_COLOR,   stopColors);
  Visual::Base gradientVisual = factory.CreateVisual(propertyMap);
  gradientVisual.SetTransformAndSize(DefaultTransform(), controlSize );
  gradientVisual.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2::ZERO,TEST_LOCATION );

  // svg visual
  Visual::Base svgVisual = factory.CreateVisual( TEST_SVG_FILE_NAME, ImageDimensions() );
  svgVisual.SetTransformAndSize(DefaultTransform(), controlSize );
  svgVisual.GetNaturalSize(naturalSize);
  // TEST_SVG_FILE:
  //  <svg width="100" height="100">
  //  <circle cx="50" cy="50" r="40" stroke="green" stroke-width="4" fill="yellow" />
  //  </svg>
  DALI_TEST_EQUALS( naturalSize, Vector2(100.f, 100.f), TEST_LOCATION );

  // svg visual with a size
  Visual::Base svgVisual2 = factory.CreateVisual( TEST_SVG_FILE_NAME, ImageDimensions(200, 200) );
  svgVisual2.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2(100.f, 100.f), TEST_LOCATION ); // Natural size should still be 100, 100

  // Text visual.

  // Load some fonts to get the same metrics on different platforms.
  TextAbstraction::FontClient fontClient = TextAbstraction::FontClient::Get();
  fontClient.SetDpi( 96u, 96u );

  char* pathNamePtr = get_current_dir_name();
  const std::string pathName( pathNamePtr );
  free( pathNamePtr );

  fontClient.GetFontId( pathName + DEFAULT_FONT_DIR + "/tizen/TizenSansRegular.ttf" );

  propertyMap.Clear();
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::TEXT );
  propertyMap.Insert( TextVisual::Property::ENABLE_MARKUP, true );
  propertyMap.Insert( TextVisual::Property::TEXT, "<font family='TizenSans' size='12'>Hello world</font>" );
  propertyMap.Insert( TextVisual::Property::MULTI_LINE, true );

  Visual::Base textVisual = factory.CreateVisual( propertyMap );
  textVisual.GetNaturalSize( naturalSize );
  DALI_TEST_EQUALS( naturalSize, Size( 80.f, 20.f ), TEST_LOCATION );

  const float height = textVisual.GetHeightForWidth( 40.f );
  DALI_TEST_EQUALS( height, 40.f, Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  //AnimatedImageVisual
  Visual::Base animatedImageVisual = factory.CreateVisual( TEST_GIF_FILE_NAME, ImageDimensions() );
  animatedImageVisual.SetTransformAndSize(DefaultTransform(), controlSize );
  animatedImageVisual.GetNaturalSize(naturalSize);
  // TEST_GIF_FILE: anim.gif
  // resolution: 50*50, frame count: 4, frame delay: 0.2 second for each frame
  DALI_TEST_EQUALS( naturalSize, Vector2(50.f, 50.f), TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualSetOnOffStage(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetDepthIndex" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );

  actor.SetSize(200.f, 200.f);

  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 0u );

  Stage::GetCurrent().Add( actor );

  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 1u );

  Stage::GetCurrent().Remove( actor );

  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 0u );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap1(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap1: ColorVisual (With base MixColor" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(DevelVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base colorVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  colorVisual.CreatePropertyMap( resultMap );

  Property::Value* typeValue = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( typeValue );
  DALI_TEST_CHECK( typeValue->Get<int>() == Visual::COLOR );

  Property::Value* colorValue = resultMap.Find( ColorVisual::Property::MIX_COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::BLUE );

  // change the blend color
  propertyMap[ColorVisual::Property::MIX_COLOR] = Color::CYAN;
  colorVisual = factory.CreateVisual( propertyMap  );
  colorVisual.CreatePropertyMap( resultMap );

  colorValue = resultMap.Find( ColorVisual::Property::MIX_COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::CYAN );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap2(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap2: BorderVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::BORDER);
  propertyMap.Insert("mixColor", Vector4(1.0f, 0.0f, 1.0f, 0.5f) );
  propertyMap.Insert("borderColor",  Color::BLUE);
  propertyMap.Insert("borderSize",  5.f);
  propertyMap.Insert("antiAliasing",  true);
  Visual::Base borderVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  borderVisual.CreatePropertyMap( resultMap );

  // check the property values from the returned map from visual
  Property::Value* typeValue = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( typeValue );
  DALI_TEST_CHECK( typeValue->Get<int>() == Visual::BORDER );

  Property::Value* colorValue = resultMap.Find( BorderVisual::Property::COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::BLUE );

  Property::Value* sizeValue = resultMap.Find( BorderVisual::Property::SIZE,  Property::FLOAT );
  DALI_TEST_CHECK( sizeValue );
  DALI_TEST_CHECK( sizeValue->Get<float>() == 5.f );

  Property::Value* AAValue = resultMap.Find( BorderVisual::Property::ANTI_ALIASING, Property::BOOLEAN );
  DALI_TEST_CHECK( AAValue );
  DALI_TEST_CHECK( AAValue->Get<bool>() == true );

  Property::Map propertyMap1;
  propertyMap1[ Visual::Property::TYPE ] = Visual::BORDER;
  propertyMap1[ BorderVisual::Property::COLOR  ] = Color::CYAN;
  propertyMap1[ BorderVisual::Property::SIZE   ] = 10.0f;
  borderVisual = factory.CreateVisual( propertyMap1 );
  borderVisual.CreatePropertyMap( resultMap );

  typeValue = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( typeValue );
  DALI_TEST_CHECK( typeValue->Get<int>() == Visual::BORDER );

  colorValue = resultMap.Find( BorderVisual::Property::COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::CYAN );

  colorValue = resultMap.Find( BorderVisual::Property::SIZE,  Property::FLOAT );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<float>() == 10.f );


  END_TEST;
}

int UtcDaliVisualGetPropertyMap2N(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap2N: BorderVisual with no setup properties" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::BORDER);
  Visual::Base borderVisual = factory.CreateVisual( propertyMap );

  tet_infoline( "Test that the visual is created, with a default renderer" );
  DALI_TEST_CHECK( borderVisual );

  DummyControl dummyControl = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(dummyControl.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, borderVisual );
  Stage::GetCurrent().Add( dummyControl );

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 1, TEST_LOCATION );

  END_TEST;
}


int UtcDaliVisualGetPropertyMap3(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap3: linear GradientVisual" );

  VisualFactory factory = VisualFactory::Get();
  DALI_TEST_CHECK( factory );

  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::GRADIENT);

  Vector2 start(-1.f, -1.f);
  Vector2 end(1.f, 1.f);
  propertyMap.Insert( "startPosition", start);
  propertyMap.Insert( "endPosition", end);
  propertyMap.Insert( "spreadMethod", GradientVisual::SpreadMethod::REPEAT);

  propertyMap.Insert(GradientVisual::Property::STOP_OFFSET,   Vector2(0.2f, 0.8f));

  Property::Array stopColors;
  stopColors.PushBack( Color::RED );
  stopColors.PushBack( Color::GREEN );
  propertyMap.Insert(GradientVisual::Property::STOP_COLOR,   stopColors);

  Visual::Base gradientVisual = factory.CreateVisual(propertyMap);

  Property::Map resultMap;
  gradientVisual.CreatePropertyMap( resultMap );

  // check the property values from the returned map from visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == Visual::GRADIENT );

  value = resultMap.Find( GradientVisual::Property::UNITS,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == GradientVisual::Units::OBJECT_BOUNDING_BOX );

  value = resultMap.Find( GradientVisual::Property::SPREAD_METHOD,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == GradientVisual::SpreadMethod::REPEAT );

  value = resultMap.Find( GradientVisual::Property::START_POSITION,   Property::VECTOR2 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector2>(), start , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( GradientVisual::Property::END_POSITION,   Property::VECTOR2 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector2>(), end , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( GradientVisual::Property::STOP_OFFSET,   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* offsetArray = value->GetArray();
  DALI_TEST_CHECK( offsetArray->Count() == 2 );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(0).Get<float>(), 0.2f , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(1).Get<float>(), 0.8f , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( GradientVisual::Property::STOP_COLOR,   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* colorArray = value->GetArray();
  DALI_TEST_CHECK( colorArray->Count() == 2 );
  DALI_TEST_EQUALS( colorArray->GetElementAt(0).Get<Vector4>(), Color::RED , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( colorArray->GetElementAt(1).Get<Vector4>(), Color::GREEN , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap4(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap4: radial GradientVisual" );

  VisualFactory factory = VisualFactory::Get();
  DALI_TEST_CHECK( factory );

  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::GRADIENT);

  Vector2 center(100.f, 100.f);
  float radius = 100.f;
  propertyMap.Insert(GradientVisual::Property::UNITS,  GradientVisual::Units::USER_SPACE);
  propertyMap.Insert(GradientVisual::Property::CENTER,  center);
  propertyMap.Insert(GradientVisual::Property::RADIUS,  radius);
  propertyMap.Insert(GradientVisual::Property::STOP_OFFSET,   Vector3(0.1f, 0.3f, 1.1f));

  Property::Array stopColors;
  stopColors.PushBack( Color::RED );
  stopColors.PushBack( Color::BLACK );
  stopColors.PushBack( Color::GREEN );
  propertyMap.Insert(GradientVisual::Property::STOP_COLOR,   stopColors);

  Visual::Base gradientVisual = factory.CreateVisual(propertyMap);
  DALI_TEST_CHECK( gradientVisual );

  Property::Map resultMap;
  gradientVisual.CreatePropertyMap( resultMap );

  // check the property values from the returned map from visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == Visual::GRADIENT );

  value = resultMap.Find( GradientVisual::Property::UNITS,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == GradientVisual::Units::USER_SPACE );

  value = resultMap.Find( GradientVisual::Property::SPREAD_METHOD,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == GradientVisual::SpreadMethod::PAD );

  value = resultMap.Find( GradientVisual::Property::CENTER,  Property::VECTOR2 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector2>(), center , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( GradientVisual::Property::RADIUS,  Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), radius , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( GradientVisual::Property::STOP_OFFSET,   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* offsetArray = value->GetArray();
  DALI_TEST_CHECK( offsetArray->Count() == 3 );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(0).Get<float>(), 0.1f , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(1).Get<float>(), 0.3f , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  // any stop value will be clamped to [0.0, 1.0];
  DALI_TEST_EQUALS( offsetArray->GetElementAt(2).Get<float>(), 1.0f , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( GradientVisual::Property::STOP_COLOR,   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* colorArray = value->GetArray();
  DALI_TEST_CHECK( colorArray->Count() == 3 );
  DALI_TEST_EQUALS( colorArray->GetElementAt(0).Get<Vector4>(), Color::RED , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( colorArray->GetElementAt(1).Get<Vector4>(), Color::BLACK , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( colorArray->GetElementAt(2).Get<Vector4>(), Color::GREEN , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap5(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap5: ImageVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE,  Visual::IMAGE );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR, Color::MAGENTA );
  propertyMap.Insert( ImageVisual::Property::URL,  TEST_IMAGE_FILE_NAME );
  propertyMap.Insert( ImageVisual::Property::DESIRED_WIDTH,   20 );
  propertyMap.Insert( ImageVisual::Property::DESIRED_HEIGHT,   30 );
  propertyMap.Insert( "fittingMode",   FittingMode::FIT_HEIGHT );
  propertyMap.Insert( "samplingMode",   SamplingMode::BOX_THEN_NEAREST );
  propertyMap.Insert( "pixelArea", Vector4( 0.25f, 0.25f, 0.5f, 0.5f ) );
  propertyMap.Insert( "wrapModeU", WrapMode::REPEAT );
  propertyMap.Insert( "wrapModeV", WrapMode::MIRRORED_REPEAT );
  propertyMap.Insert( "synchronousLoading",   true );

  Visual::Base imageVisual = factory.CreateVisual(propertyMap);
  DALI_TEST_CHECK( imageVisual );

  Property::Map resultMap;
  imageVisual.CreatePropertyMap( resultMap );

  // check the property values from the returned map from visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == Visual::IMAGE );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_IMAGE_FILE_NAME );

  value = resultMap.Find( DevelVisual::Property::MIX_COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<Vector4>() == Color::MAGENTA );

  value = resultMap.Find( ImageVisual::Property::FITTING_MODE,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == FittingMode::FIT_HEIGHT );

  value = resultMap.Find( ImageVisual::Property::SAMPLING_MODE,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == SamplingMode::BOX_THEN_NEAREST );

  value = resultMap.Find( ImageVisual::Property::DESIRED_WIDTH,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 20 );

  value = resultMap.Find( ImageVisual::Property::DESIRED_HEIGHT,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 30 );

  value = resultMap.Find( ImageVisual::Property::PIXEL_AREA, Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector4>(), Vector4( 0.25f, 0.25f, 0.5f, 0.5f ), Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( ImageVisual::Property::WRAP_MODE_U, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK(  value->Get<int>() == WrapMode::REPEAT);

  value = resultMap.Find( ImageVisual::Property::WRAP_MODE_V, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK(  value->Get<int>() == WrapMode::MIRRORED_REPEAT);

  value = resultMap.Find( "synchronousLoading",   Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<bool>() == true );

  // Get an image visual with an image handle, and test the default property values
  PrepareResourceImage( application, 100u, 200u, Pixel::RGBA8888 );
  Image image = ResourceImage::New(TEST_IMAGE_FILE_NAME, ImageDimensions(100, 200));
  imageVisual = factory.CreateVisual(image);
  imageVisual.CreatePropertyMap( resultMap );

  value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == Visual::IMAGE );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_IMAGE_FILE_NAME );

  value = resultMap.Find( ImageVisual::Property::FITTING_MODE,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == FittingMode::SHRINK_TO_FIT );

  value = resultMap.Find( ImageVisual::Property::SAMPLING_MODE,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == SamplingMode::BOX );

  value = resultMap.Find( ImageVisual::Property::DESIRED_WIDTH,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 100 );

  value = resultMap.Find( ImageVisual::Property::DESIRED_HEIGHT,   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 200 );

  value = resultMap.Find( ImageVisual::Property::PIXEL_AREA, Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector4>(), Vector4( 0.f, 0.f, 1.f, 1.f ), Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( ImageVisual::Property::WRAP_MODE_U, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK(  value->Get<int>() == WrapMode::DEFAULT);

  value = resultMap.Find( ImageVisual::Property::WRAP_MODE_V, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK(  value->Get<int>() == WrapMode::DEFAULT);

  value = resultMap.Find( "synchronousLoading",   Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<bool>() == false );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap6(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap6: NPatchVisual" );

  Rect< int > border( 1, 1, 1, 1 );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::N_PATCH );
  propertyMap.Insert( "mixColor",  Color::MAGENTA );
  propertyMap.Insert( ImageVisual::Property::URL,  TEST_NPATCH_FILE_NAME );
  propertyMap.Insert( ImageVisual::Property::BORDER_ONLY,  true );
  propertyMap.Insert( DevelImageVisual::Property::BORDER, border );
  Visual::Base nPatchVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  nPatchVisual.CreatePropertyMap( resultMap );

  // check the property values from the returned map from visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == DevelVisual::N_PATCH );

  value = resultMap.Find( DevelVisual::Property::MIX_COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<Vector4>() == Color::MAGENTA );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_NPATCH_FILE_NAME );

  value = resultMap.Find( ImageVisual::Property::BORDER_ONLY,  Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<bool>() );

  value = resultMap.Find( DevelImageVisual::Property::BORDER,  Property::RECTANGLE );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get< Rect< int > >() == border );

  Vector4 border1( 1.0f, 1.0f, 1.0f, 1.0f );

  Property::Map propertyMap1;
  propertyMap1.Insert( Visual::Property::TYPE, DevelVisual::N_PATCH );
  propertyMap1.Insert( "mixColor",  Color::MAGENTA );
  propertyMap1.Insert( ImageVisual::Property::URL,  TEST_NPATCH_FILE_NAME );
  propertyMap1.Insert( ImageVisual::Property::BORDER_ONLY,  true );
  propertyMap1.Insert( DevelImageVisual::Property::BORDER, border1 );
  nPatchVisual = factory.CreateVisual( propertyMap1 );

  nPatchVisual.CreatePropertyMap( resultMap );

  // check the property values from the returned map from visual
  value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == DevelVisual::N_PATCH );

  value = resultMap.Find( DevelVisual::Property::MIX_COLOR,  Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<Vector4>() == Color::MAGENTA );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_NPATCH_FILE_NAME );

  value = resultMap.Find( ImageVisual::Property::BORDER_ONLY,  Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<bool>() );

  value = resultMap.Find( DevelImageVisual::Property::BORDER,  Property::RECTANGLE );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get< Rect< int > >() == border );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap7(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap7: SvgVisual" );

  // request SvgVisual with a property map
  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::SVG );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR, Color::WHITE );
  propertyMap.Insert( ImageVisual::Property::URL, TEST_SVG_FILE_NAME );
  Visual::Base svgVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  svgVisual.CreatePropertyMap( resultMap );
  // check the property values from the returned map from a visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == DevelVisual::SVG );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_SVG_FILE_NAME );

  // request SvgVisual with an URL
  Visual::Base svgVisual2 = factory.CreateVisual( TEST_SVG_FILE_NAME, ImageDimensions() );
  resultMap.Clear();
  svgVisual2.CreatePropertyMap( resultMap );
  // check the property values from the returned map from a visual
  value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == DevelVisual::SVG );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_SVG_FILE_NAME );

  END_TEST;
}

//Mesh visual
int UtcDaliVisualGetPropertyMap8(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap8: MeshVisual" );

  //Request MeshVisual using a property map.
  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, Visual::MESH );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR, Color::BLUE );
  propertyMap.Insert( MeshVisual::Property::OBJECT_URL, TEST_OBJ_FILE_NAME );
  propertyMap.Insert( MeshVisual::Property::MATERIAL_URL, TEST_MTL_FILE_NAME );
  propertyMap.Insert( MeshVisual::Property::TEXTURES_PATH, TEST_RESOURCE_LOCATION );
  propertyMap.Insert( MeshVisual::Property::SHADING_MODE, MeshVisual::ShadingMode::TEXTURELESS_WITH_DIFFUSE_LIGHTING );
  propertyMap.Insert( MeshVisual::Property::LIGHT_POSITION, Vector3( 5.0f, 10.0f, 15.0f) );
  Visual::Base meshVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  meshVisual.CreatePropertyMap( resultMap );
  TestMixColor( meshVisual, DevelVisual::Property::MIX_COLOR, Color::BLUE );

  //Check values in the result map are identical to the initial map's values.
  Property::Value* value = resultMap.Find( Visual::Property::TYPE, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), (int)Visual::MESH, TEST_LOCATION );

  value = resultMap.Find( MeshVisual::Property::OBJECT_URL, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), TEST_OBJ_FILE_NAME, TEST_LOCATION );

  value = resultMap.Find( MeshVisual::Property::MATERIAL_URL, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), TEST_MTL_FILE_NAME, TEST_LOCATION );

  value = resultMap.Find( MeshVisual::Property::TEXTURES_PATH, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), TEST_RESOURCE_LOCATION, TEST_LOCATION );

  value = resultMap.Find( MeshVisual::Property::SHADING_MODE, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), (int)MeshVisual::ShadingMode::TEXTURELESS_WITH_DIFFUSE_LIGHTING, TEST_LOCATION );

  value = resultMap.Find( MeshVisual::Property::LIGHT_POSITION, Property::VECTOR3 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector3>(), Vector3( 5.0f, 10.0f, 15.0f), Math::MACHINE_EPSILON_100, TEST_LOCATION );

 END_TEST;
}

//Primitive shape visual
int UtcDaliVisualGetPropertyMap9(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap9: PrimitiveVisual" );

  Vector4 color = Vector4( 1.0, 0.8, 0.6, 1.0);
  Vector3 dimensions = Vector3( 1.0, 2.0, 3.0 );

  //Request PrimitiveVisual using a property map.
  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, Visual::PRIMITIVE );
  propertyMap.Insert( PrimitiveVisual::Property::SHAPE, PrimitiveVisual::Shape::CUBE );
  propertyMap.Insert( PrimitiveVisual::Property::MIX_COLOR, color );
  propertyMap.Insert( PrimitiveVisual::Property::SLICES, 10 );
  propertyMap.Insert( PrimitiveVisual::Property::STACKS, 20 );
  propertyMap.Insert( PrimitiveVisual::Property::SCALE_TOP_RADIUS, 30.0f );
  propertyMap.Insert( PrimitiveVisual::Property::SCALE_BOTTOM_RADIUS, 40.0f );
  propertyMap.Insert( PrimitiveVisual::Property::SCALE_HEIGHT, 50.0f );
  propertyMap.Insert( PrimitiveVisual::Property::SCALE_RADIUS, 60.0f );
  propertyMap.Insert( PrimitiveVisual::Property::SCALE_DIMENSIONS, dimensions );
  propertyMap.Insert( PrimitiveVisual::Property::BEVEL_PERCENTAGE, 0.3f );
  propertyMap.Insert( PrimitiveVisual::Property::BEVEL_SMOOTHNESS, 0.6f );
  propertyMap.Insert( PrimitiveVisual::Property::LIGHT_POSITION, Vector3( 5.0f, 10.0f, 15.0f) );
  Visual::Base primitiveVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  primitiveVisual.CreatePropertyMap( resultMap );

  //Check values in the result map are identical to the initial map's values.
  Property::Value* value = resultMap.Find( Visual::Property::TYPE, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), (int)Visual::PRIMITIVE, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SHAPE, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), (int)PrimitiveVisual::Shape::CUBE, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::MIX_COLOR, Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<Vector4>() == color );
  DALI_TEST_EQUALS( value->Get<Vector4>(), color, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SLICES, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), 10, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::STACKS, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), 20, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SCALE_TOP_RADIUS, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 30.0f, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SCALE_BOTTOM_RADIUS, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 40.0f, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SCALE_HEIGHT, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 50.0f, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SCALE_RADIUS, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 60.0f, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::SCALE_DIMENSIONS, Property::VECTOR3 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector3>(), dimensions, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::BEVEL_PERCENTAGE, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 0.3f, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::BEVEL_SMOOTHNESS, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 0.6f, Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( PrimitiveVisual::Property::LIGHT_POSITION, Property::VECTOR3 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector3>(), Vector3( 5.0f, 10.0f, 15.0f), Math::MACHINE_EPSILON_100, TEST_LOCATION );

  END_TEST;
}

//Text shape visual
int UtcDaliVisualGetPropertyMap10(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap10: TextVisual" );

  //Request PrimitiveVisual using a property map.
  VisualFactory factory = VisualFactory::Get();

  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::TEXT );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR, Color::BLACK );
  propertyMap.Insert( "renderingBackend", static_cast<int>( Toolkit::Text::DEFAULT_RENDERING_BACKEND ) );
  propertyMap.Insert( "enableMarkup", false );
  propertyMap.Insert( "text", "Hello world" );
  propertyMap.Insert( "fontFamily", "TizenSans" );

  Property::Map fontStyleMapSet;
  fontStyleMapSet.Insert( "weight", "bold" );
  propertyMap.Insert( "fontStyle", fontStyleMapSet );

  propertyMap.Insert( "pointSize", 12.f );
  propertyMap.Insert( "multiLine", true );
  propertyMap.Insert( "horizontalAlignment", "CENTER" );
  propertyMap.Insert( "verticalAlignment", "CENTER" );
  propertyMap.Insert( "textColor", Color::RED );
  Visual::Base textVisual = factory.CreateVisual( propertyMap );

  Property::Map resultMap;
  textVisual.CreatePropertyMap( resultMap );

  //Check values in the result map are identical to the initial map's values.
  Property::Value* value = resultMap.Find( Visual::Property::TYPE, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<int>(), (int)DevelVisual::TEXT, TEST_LOCATION );

  value = resultMap.Find( DevelVisual::Property::MIX_COLOR, Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector4>(), Color::BLACK, 0.001f, TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::TEXT, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), "Hello world", TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::FONT_FAMILY, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), "TizenSans", TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::FONT_STYLE, Property::MAP );
  DALI_TEST_CHECK( value );

  Property::Map fontStyleMapGet = value->Get<Property::Map>();
  DALI_TEST_EQUALS( fontStyleMapGet.Count(), fontStyleMapSet.Count(), TEST_LOCATION );
  DALI_TEST_EQUALS( DaliTestCheckMaps( fontStyleMapGet, fontStyleMapSet ), true, TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::POINT_SIZE, Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), 12.f, Math::MACHINE_EPSILON_1000, TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::MULTI_LINE, Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<bool>() );

  value = resultMap.Find( TextVisual::Property::HORIZONTAL_ALIGNMENT, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), "CENTER", TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::VERTICAL_ALIGNMENT, Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<std::string>(), "CENTER", TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::TEXT_COLOR, Property::VECTOR4 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector4>(), Color::RED, TEST_LOCATION );

  value = resultMap.Find( TextVisual::Property::ENABLE_MARKUP, Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( !value->Get<bool>() );

  END_TEST;
}

int UtcDaliVisualGetPropertyMap11(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetPropertyMap11: AnimatedImageVisual" );

  // request AnimatedImageVisual with a property map
  VisualFactory factory = VisualFactory::Get();
  Visual::Base animatedImageVisual = factory.CreateVisual( Property::Map()
                                                 .Add( Visual::Property::TYPE, DevelVisual::ANIMATED_IMAGE )
                                                 .Add( ImageVisual::Property::URL, TEST_GIF_FILE_NAME ) );

  Property::Map resultMap;
  animatedImageVisual.CreatePropertyMap( resultMap );
  // check the property values from the returned map from a visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == DevelVisual::ANIMATED_IMAGE );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_GIF_FILE_NAME );

  // request AnimatedImageVisual with an URL
  Visual::Base animatedImageVisual2 = factory.CreateVisual( TEST_GIF_FILE_NAME, ImageDimensions() );
  resultMap.Clear();
  animatedImageVisual2.CreatePropertyMap( resultMap );
  // check the property values from the returned map from a visual
  value = resultMap.Find( Visual::Property::TYPE,  Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == DevelVisual::ANIMATED_IMAGE );

  value = resultMap.Find( ImageVisual::Property::URL,  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_GIF_FILE_NAME );

  END_TEST;
}

int UtcDaliVisualAnimateBorderVisual01(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimateBorderVisual Color" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::BORDER);
  propertyMap.Insert(DevelVisual::Property::MIX_COLOR, Vector4(1, 1, 1, 0.8f));
  propertyMap.Insert(BorderVisual::Property::COLOR,  Color::BLUE);
  propertyMap.Insert(BorderVisual::Property::SIZE,  5.f);
  Visual::Base borderVisual = factory.CreateVisual( propertyMap );

  Property::Map map;
  map["target"] = "testVisual";
  map["property"] = "mixColor";
  map["targetValue"] = Vector4(1,1,1,0);
  map["animator"] = Property::Map()
    .Add("alphaFunction", "LINEAR")
    .Add("timePeriod", Property::Map()
         .Add("delay", 0.0f)
         .Add("duration", 4.0f));

  Dali::Toolkit::TransitionData transition = TransitionData::New( map );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, borderVisual );
  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  Stage::GetCurrent().Add(actor);

  DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

  Renderer renderer = actor.GetRendererAt(0);
  Property::Index borderColorIndex = DevelHandle::GetPropertyIndex( renderer, BorderVisual::Property::COLOR );
  Property::Index mixColorIndex = DevelHandle::GetPropertyIndex( renderer, DevelVisual::Property::MIX_COLOR );

  Animation animation = dummyImpl.CreateTransition( transition );

  // Animate the mix color through the transition, and the border color through
  // programmatic method.
  animation.AnimateTo( Property(renderer, borderColorIndex), Color::WHITE );
  animation.Play();

  application.SendNotification();
  application.Render(0);
  application.Render(2000u); // halfway point between blue and white

  Vector4 color = renderer.GetProperty<Vector4>( borderColorIndex );
  Vector4 testColor = (Color::BLUE + Color::WHITE)*0.5f;
  DALI_TEST_EQUALS( color, testColor, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("borderColor", testColor ), true, TEST_LOCATION );

  color = renderer.GetProperty<Vector3>( mixColorIndex );
  testColor = Vector4( 1,1,1,0.4f );
  DALI_TEST_EQUALS( Vector3(color), Vector3(testColor), 0.0001f, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(testColor) ), true, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", testColor.a ) , true, TEST_LOCATION );

  application.Render(2000u);

  color = renderer.GetProperty<Vector4>( borderColorIndex );
  DALI_TEST_EQUALS( color, Color::WHITE, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("borderColor", Color::WHITE ), true, TEST_LOCATION );

  color = renderer.GetProperty<Vector4>( mixColorIndex );
  testColor = Vector4(1,1,1,0);
  DALI_TEST_EQUALS( color, testColor, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(testColor) ), true, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", testColor.a ), true, TEST_LOCATION );

  END_TEST;
}


int UtcDaliVisualAnimateBorderVisual02(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimateBorderVisual Size" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::BORDER);
  propertyMap.Insert(BorderVisual::Property::COLOR,  Color::BLUE);
  propertyMap.Insert(BorderVisual::Property::SIZE,  5.f);
  Visual::Base borderVisual = factory.CreateVisual( propertyMap );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, borderVisual );
  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  Stage::GetCurrent().Add(actor);

  DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

  Renderer renderer = actor.GetRendererAt(0);
  Property::Index index = DevelHandle::GetPropertyIndex( renderer, BorderVisual::Property::SIZE );

  Animation animation = Animation::New(4.0f);
  animation.AnimateTo( Property(renderer, index), 9.0f );
  animation.Play();

  application.SendNotification();
  application.Render(0);
  application.Render(2000u); // halfway point

  float size = renderer.GetProperty<float>( index );
  DALI_TEST_EQUALS( size, 7.0f, 0.0001f, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("borderSize", 7.0f ), true, TEST_LOCATION );

  application.Render(2000u); // halfway point between blue and white

  size = renderer.GetProperty<float>( index );
  DALI_TEST_EQUALS( size, 9.0f, 0.0001f, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("borderSize", 9.0f ), true, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualAnimateColorVisual(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimateColorVisual mixColor" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR, Color::BLUE);
  Visual::Base borderVisual = factory.CreateVisual( propertyMap );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, borderVisual );
  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  Stage::GetCurrent().Add(actor);

  DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

  Renderer renderer = actor.GetRendererAt(0);
  Property::Index mixColorIndex = DevelHandle::GetPropertyIndex( renderer, ColorVisual::Property::MIX_COLOR );

  Property::Value blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
  DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::AUTO, TEST_LOCATION );

  Animation animation = Animation::New(4.0f);
  animation.AnimateTo( Property(renderer, mixColorIndex), Vector3(Color::WHITE) );
  animation.Play();

  application.SendNotification();
  application.Render(0);
  application.Render(2000u); // halfway point

  Vector3 color = renderer.GetProperty<Vector3>( mixColorIndex );
  Vector3 testColor = Vector3(Color::BLUE + Color::WHITE)*0.5f;
  DALI_TEST_EQUALS( color, testColor, TEST_LOCATION );

  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", testColor ), true, TEST_LOCATION );

  application.Render(2000u); // halfway point between blue and white

  color = renderer.GetProperty<Vector3>( mixColorIndex );
  DALI_TEST_EQUALS( color, Vector3(Color::WHITE), TEST_LOCATION );

  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(Color::WHITE) ), true, TEST_LOCATION );

  blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
  DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::AUTO, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualAnimatePrimitiveVisual(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimatePrimitiveVisual color" );

  {
    VisualFactory factory = VisualFactory::Get();
    Property::Map propertyMap;
    propertyMap.Insert(Visual::Property::TYPE,  Visual::PRIMITIVE);
    propertyMap.Insert(PrimitiveVisual::Property::SHAPE, PrimitiveVisual::Shape::CUBE);
    propertyMap.Insert(PrimitiveVisual::Property::MIX_COLOR, Color::BLUE);
    Visual::Base visual = factory.CreateVisual( propertyMap );

    DummyControl actor = DummyControl::New(true);
    Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
    dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );
    actor.SetSize(2000, 2000);
    actor.SetParentOrigin(ParentOrigin::CENTER);
    actor.SetColor(Color::BLACK);
    Stage::GetCurrent().Add(actor);

    DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

    Renderer renderer = actor.GetRendererAt(0);
    Property::Index index = DevelHandle::GetPropertyIndex( renderer, PrimitiveVisual::Property::MIX_COLOR );

    tet_infoline("Test that the renderer has the Primitive mix color");
    DALI_TEST_CHECK( index != Property::INVALID_INDEX );

    const Vector4 INITIAL_MIX_COLOR( 1.0f,0.0f,1.0f,0.5f ); // Magenta with half alpha
    const Vector4 TARGET_MIX_COLOR( Color::RED );

    Property::Map map;
    map["target"] = "testVisual";
    map["property"] = "mixColor";
    map["initialValue"] = INITIAL_MIX_COLOR;
    map["targetValue"] = TARGET_MIX_COLOR;
    map["animator"] = Property::Map()
      .Add("alphaFunction", "LINEAR")
      .Add("timePeriod", Property::Map()
           .Add("delay", 0.0f)
           .Add("duration", 4.0f));

    Dali::Toolkit::TransitionData transition = TransitionData::New( map );

    Animation animation = dummyImpl.CreateTransition( transition );
    Property::Value blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
    DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::ON, TEST_LOCATION );

    animation.AnimateTo( Property(actor, Actor::Property::COLOR), Color::WHITE );
    animation.Play();

    application.SendNotification();
    application.Render(0);
    application.Render(2000u); // halfway point
    application.SendNotification();

    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("uColor", Vector4(0.5f, 0.5f, 0.5f, 1.0f )), true, TEST_LOCATION );

    Vector4 halfwayColor = (INITIAL_MIX_COLOR + TARGET_MIX_COLOR)*0.5;
    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(halfwayColor) ), true, TEST_LOCATION );
    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", halfwayColor.a ), true, TEST_LOCATION );

    application.Render(2001u); // go past end
    application.SendNotification(); // Trigger signals

    DALI_TEST_EQUALS( actor.GetCurrentColor(), Color::WHITE, TEST_LOCATION );
    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("uColor", Color::WHITE ), true, TEST_LOCATION );
    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(TARGET_MIX_COLOR) ), true, TEST_LOCATION );
    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", TARGET_MIX_COLOR.a ), true, TEST_LOCATION );

    blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
    DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::AUTO, TEST_LOCATION );

    actor.Unparent();
  }

  END_TEST;
}

int UtcDaliVisualAnimateImageVisualMixColor(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimateImageVisual mix color" );

  application.GetPlatform().SetClosestImageSize( Vector2(100, 100) );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::IMAGE);
  propertyMap.Insert(ImageVisual::Property::URL, TEST_IMAGE_FILE_NAME );
  propertyMap.Insert("mixColor", Color::BLUE);
  propertyMap.Insert(ImageVisual::Property::SYNCHRONOUS_LOADING, true);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );

  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  actor.SetColor(Color::BLACK);
  Stage::GetCurrent().Add(actor);

  DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

  Renderer renderer = actor.GetRendererAt(0);
  Property::Index index = DevelHandle::GetPropertyIndex( renderer, DevelVisual::Property::MIX_COLOR );
  Property::Value blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
  DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::AUTO, TEST_LOCATION );

  tet_infoline("Test that the renderer has the mixColor property");
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );

  const Vector4 TARGET_MIX_COLOR( 1.0f, 0.0f, 0.0f, 0.5f );

  Property::Map map;
  map["target"] = "testVisual";
  map["property"] = "mixColor";
  map["initialValue"] = Color::MAGENTA;
  map["targetValue"] = TARGET_MIX_COLOR;
  map["animator"] = Property::Map()
    .Add("alphaFunction", "LINEAR")
    .Add("timePeriod", Property::Map()
         .Add("delay", 0.0f)
         .Add("duration", 4.0f));

  Dali::Toolkit::TransitionData transition = TransitionData::New( map );

  Animation animation = dummyImpl.CreateTransition( transition );

  blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
  DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::ON, TEST_LOCATION );

  animation.AnimateTo( Property(actor, Actor::Property::COLOR), Color::WHITE );
  animation.Play();

  application.SendNotification();
  application.Render(0);
  application.Render(2000u); // halfway point
  Vector4 testColor(1.0f, 0.0f, 0.5f, 0.75f );

  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("uColor", Vector4(0.5f, 0.5f, 0.5f, 1.0f )), true, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(testColor)), true, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", testColor.a), true, TEST_LOCATION );

  application.Render(2000u); // halfway point between blue and white

  DALI_TEST_EQUALS( actor.GetCurrentColor(), Color::WHITE, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("uColor", Color::WHITE ), true, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector3>("mixColor", Vector3(TARGET_MIX_COLOR)), true, TEST_LOCATION );
  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", TARGET_MIX_COLOR.a), true, TEST_LOCATION );

  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, TARGET_MIX_COLOR );

  blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
  DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::ON, TEST_LOCATION );

  END_TEST;
}


int UtcDaliVisualAnimateImageVisualOpacity(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimateImageVisual mix color" );

  application.GetPlatform().SetClosestImageSize( Vector2(100, 100) );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::IMAGE);
  propertyMap.Insert(ImageVisual::Property::URL, TEST_IMAGE_FILE_NAME );
  propertyMap.Insert("opacity", 0.5f);
  propertyMap.Insert(ImageVisual::Property::SYNCHRONOUS_LOADING, true);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );

  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  actor.SetColor(Color::BLACK);
  Stage::GetCurrent().Add(actor);

  DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

  Renderer renderer = actor.GetRendererAt(0);
  tet_infoline("Test that the renderer has the opacity property");
  Property::Index index = DevelHandle::GetPropertyIndex( renderer, DevelVisual::Property::OPACITY );
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );


  Property::Value blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
  DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::ON, TEST_LOCATION );

  {
    tet_infoline( "Test that the opacity can be increased to full via animation, and that the blend mode is set appropriately at the start and end of the animation." );

    Property::Map map;
    map["target"] = "testVisual";
    map["property"] = "opacity";
    map["targetValue"] = 1.0f;
    map["animator"] = Property::Map()
      .Add("alphaFunction", "LINEAR")
      .Add("timePeriod", Property::Map()
           .Add("delay", 0.0f)
           .Add("duration", 4.0f));

    Dali::Toolkit::TransitionData transition = TransitionData::New( map );
    Animation animation = dummyImpl.CreateTransition( transition );
    animation.Play();

    application.SendNotification();
    application.Render(0);
    application.Render(2000u); // halfway point
    application.SendNotification();

    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", 0.75f), true, TEST_LOCATION );

    application.Render(2001u); // end
    application.SendNotification();

    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", 1.0f), true, TEST_LOCATION );

    blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
    DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::AUTO, TEST_LOCATION );
  }


  {
    tet_infoline( "Test that the opacity can be reduced via animation, and that the blend mode is set appropriately at the start and end of the animation." );

    Property::Map map;
    map["target"] = "testVisual";
    map["property"] = DevelVisual::Property::OPACITY;
    map["targetValue"] = 0.1f;
    map["animator"] = Property::Map()
      .Add("alphaFunction", "LINEAR")
      .Add("timePeriod", Property::Map()
           .Add("delay", 0.0f)
           .Add("duration", 4.0f));

    Dali::Toolkit::TransitionData transition = TransitionData::New( map );
    Animation animation = dummyImpl.CreateTransition( transition );
    animation.Play();

    blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
    DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::ON, TEST_LOCATION );

    application.SendNotification();
    application.Render(0);
    application.Render(2000u); // halfway point
    application.SendNotification();

    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", 0.55f), true, TEST_LOCATION );

    application.Render(2016u); // end
    application.SendNotification();

    DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<float>("opacity", 0.1f), true, TEST_LOCATION );

    blendModeValue = renderer.GetProperty( Renderer::Property::BLEND_MODE );
    DALI_TEST_EQUALS( blendModeValue.Get<int>(), (int)BlendMode::ON, TEST_LOCATION );
  }


  END_TEST;
}

int UtcDaliVisualAnimateImageVisualPixelArea(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliAnimateImageVisual pixel area" );

  application.GetPlatform().SetClosestImageSize( Vector2(100, 100) );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::IMAGE);
  propertyMap.Insert(ImageVisual::Property::URL, TEST_IMAGE_FILE_NAME );
  propertyMap.Insert("mixColor", Color::BLUE);
  propertyMap.Insert(ImageVisual::Property::SYNCHRONOUS_LOADING, true);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );

  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  actor.SetColor(Color::BLACK);
  Stage::GetCurrent().Add(actor);

  DALI_TEST_EQUALS( actor.GetRendererCount(), 1u, TEST_LOCATION);

  Renderer renderer = actor.GetRendererAt(0);
  Property::Index index = DevelHandle::GetPropertyIndex( renderer, DevelVisual::Property::MIX_COLOR );

  tet_infoline("Test that the renderer has the mixColor property");
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );

  // TransitionData only takes string keys
  Property::Map map;
  map["target"] = "testVisual";
  map["property"] = "pixelArea";
  map["initialValue"] = Vector4( 0,0,0,1 );
  map["targetValue"] = Vector4( 0,0,1,1 ); // Animate width from zero to full
  map["animator"] = Property::Map()
    .Add("alphaFunction", "LINEAR")
    .Add("timePeriod", Property::Map()
         .Add("delay", 0.0f)
         .Add("duration", 4.0f));

  Dali::Toolkit::TransitionData transition = TransitionData::New( map );

  Animation animation = dummyImpl.CreateTransition( transition );
  animation.AnimateTo( Property(actor, Actor::Property::COLOR), Color::WHITE );
  animation.Play();

  application.SendNotification();
  application.Render(0);
  application.Render(2000u); // halfway point

  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("pixelArea", Vector4(0.0f, 0.0f, 0.5f, 1.0f )), true, TEST_LOCATION );

  application.Render(2000u);

  DALI_TEST_EQUALS( application.GetGlAbstraction().CheckUniformValue<Vector4>("pixelArea", Vector4( 0.0f, 0.0f, 1.0f, 1.0f )), true, TEST_LOCATION );

  END_TEST;
}


int UtcDaliVisualWireframeVisual(void)
{
  ToolkitTestApplication application;

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, Visual::WIREFRAME );

  // Create the visual.
  Visual::Base visual = factory.CreateVisual( propertyMap );

  DALI_TEST_CHECK( visual );

  Property::Map resultMap;
  visual.CreatePropertyMap( resultMap );

  // Check the property values from the returned map from visual
  Property::Value* value = resultMap.Find( Visual::Property::TYPE, Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == Visual::WIREFRAME );

  END_TEST;
}

int UtcDaliVisualGetTransform(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualGetTransform: ColorVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base colorVisual = factory.CreateVisual( propertyMap );

  Dali::Property::Map visualMap;
  colorVisual.CreatePropertyMap( visualMap );
  Property::Value* value = visualMap.Find( Dali::Toolkit::DevelVisual::Property::TRANSFORM );
  Dali::Property::Map* map = value->GetMap();
  DALI_TEST_CHECK( map );

  //Test default values
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::OFFSET );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_CHECK( typeValue->Get<Vector2>() == Vector2(0.0f,0.0f) );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::SIZE );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_CHECK( typeValue->Get<Vector2>() == Vector2(1.0f,1.0f) );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::OFFSET_POLICY );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_CHECK( typeValue->Get< Vector2 >() == Vector2( Toolkit::DevelVisual::Transform::Policy::RELATIVE, Toolkit::DevelVisual::Transform::Policy::RELATIVE ) );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::SIZE_POLICY );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_CHECK( typeValue->Get< Vector2 >() == Vector2( Toolkit::DevelVisual::Transform::Policy::RELATIVE, Toolkit::DevelVisual::Transform::Policy::RELATIVE ) );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::ORIGIN );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_CHECK( (Toolkit::Align::Type)typeValue->Get<int>() == Toolkit::Align::CENTER );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::ANCHOR_POINT );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_CHECK( (Toolkit::Align::Type)typeValue->Get<int>() == Toolkit::Align::CENTER );
  }

  END_TEST;
}

static void TestTransform( ToolkitTestApplication& application, Visual::Base visual )
{
  Property::Map transform;
  transform.Insert( DevelVisual::Transform::Property::OFFSET, Vector2(10.0f, 10.0f) );
  transform.Insert( DevelVisual::Transform::Property::SIZE, Vector2(0.2f, 0.2f) );
  transform.Insert( DevelVisual::Transform::Property::OFFSET_POLICY, Vector2( Toolkit::DevelVisual::Transform::Policy::ABSOLUTE, Toolkit::DevelVisual::Transform::Policy::ABSOLUTE ) );
  transform.Insert( DevelVisual::Transform::Property::ORIGIN, "TOP_BEGIN" );
  transform.Insert( DevelVisual::Transform::Property::ANCHOR_POINT, Toolkit::Align::BOTTOM_END );

  visual.SetTransformAndSize( transform, Vector2(100, 100) );

  Dali::Property::Map visualMap;
  visual.CreatePropertyMap( visualMap );
  Property::Value* value = visualMap.Find( Dali::Toolkit::DevelVisual::Property::TRANSFORM );
  Dali::Property::Map* map = value->GetMap();
  DALI_TEST_CHECK( map );

  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::OFFSET );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( typeValue->Get<Vector2>(),Vector2(10.0f,10.0f), TEST_LOCATION );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::SIZE );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( typeValue->Get<Vector2>(), Vector2(0.2f,0.2f), TEST_LOCATION );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::OFFSET_POLICY );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( typeValue->Get< Vector2 >(), Vector2( Toolkit::DevelVisual::Transform::Policy::ABSOLUTE, Toolkit::DevelVisual::Transform::Policy::ABSOLUTE ), TEST_LOCATION );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::SIZE_POLICY );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( typeValue->Get< Vector2 >(), Vector2( Toolkit::DevelVisual::Transform::Policy::RELATIVE, Toolkit::DevelVisual::Transform::Policy::RELATIVE ), TEST_LOCATION );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::ORIGIN );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( (Toolkit::Align::Type)typeValue->Get<int>(), Toolkit::Align::TOP_BEGIN, TEST_LOCATION );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::ANCHOR_POINT );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( (Toolkit::Align::Type)typeValue->Get<int>(), Toolkit::Align::BOTTOM_END, TEST_LOCATION );
  }

  //Put the visual on the stage
  DummyControl actor = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(actor.GetImplementation());
  actor.SetSize(2000, 2000);
  actor.SetParentOrigin(ParentOrigin::CENTER);
  Stage::GetCurrent().Add(actor);

  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );
  dummyImpl.SetLayout( DummyControl::Property::TEST_VISUAL, transform );

  application.SendNotification();
  application.Render(0);
  Renderer renderer( actor.GetRendererAt(0) );

  //Check that the properties have been registered on the Renderer
  Property::Index index = renderer.GetPropertyIndex( "offset" );
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );
  Vector2 offset = renderer.GetProperty<Vector2>( index );
  DALI_TEST_EQUALS( offset, Vector2(10.0f,10.0f), TEST_LOCATION );

  index = renderer.GetPropertyIndex( "size" );
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );
  Vector2 size = renderer.GetProperty<Vector2>( index );
  DALI_TEST_EQUALS( size, Vector2(0.2f,0.2f), TEST_LOCATION );

  index = renderer.GetPropertyIndex( "offsetSizeMode" );
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );
  Vector4 offsetSizeMode = renderer.GetProperty<Vector4>( index );
  DALI_TEST_EQUALS( offsetSizeMode, Vector4(1.0f,1.0f,0.0f,0.0f), TEST_LOCATION );

  index = renderer.GetPropertyIndex( "origin" );
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );
  Vector2 parentOrigin = renderer.GetProperty<Vector2>( index );
  DALI_TEST_EQUALS( parentOrigin, Vector2(-0.5f,-0.5f), TEST_LOCATION );

  index = renderer.GetPropertyIndex( "anchorPoint" );
  DALI_TEST_CHECK( index != Property::INVALID_INDEX );
  Vector2 anchorPoint = renderer.GetProperty<Vector2>( index );
  DALI_TEST_EQUALS( anchorPoint, Vector2(-0.5f,-0.5f), TEST_LOCATION );

  //Set a new transform
  transform.Clear();
  transform = DefaultTransform();
  transform.Insert( DevelVisual::Transform::Property::OFFSET, Vector2(20.0f, 20.0f) );
  transform.Insert( DevelVisual::Transform::Property::SIZE, Vector2(100.0f, 100.0f) );
  transform.Insert( DevelVisual::Transform::Property::SIZE_POLICY, Vector2( Toolkit::DevelVisual::Transform::Policy::ABSOLUTE, Toolkit::DevelVisual::Transform::Policy::ABSOLUTE ) );
  visual.SetTransformAndSize( transform, Vector2(100, 100) );
  application.SendNotification();
  application.Render(0);

  //Check that the values have changed in the renderer
  offset = renderer.GetProperty<Vector2>( renderer.GetPropertyIndex( "offset" ) );
  DALI_TEST_EQUALS( offset, Vector2(20.0f,20.0f), TEST_LOCATION );

  size = renderer.GetProperty<Vector2>( renderer.GetPropertyIndex( "size" ) );
  DALI_TEST_EQUALS( size, Vector2(100.0f,100.0f), TEST_LOCATION );

  offsetSizeMode = renderer.GetProperty<Vector4>( renderer.GetPropertyIndex( "offsetSizeMode" ) );
  DALI_TEST_EQUALS( offsetSizeMode, Vector4(0.0f,0.0f,1.0f,1.0f), TEST_LOCATION );

  //Parent origin and anchor point should have default values
  parentOrigin = renderer.GetProperty<Vector2>( renderer.GetPropertyIndex( "origin" ) );
  DALI_TEST_EQUALS( parentOrigin, Vector2(0.0f,0.0f), TEST_LOCATION );

  anchorPoint = renderer.GetProperty<Vector2>( renderer.GetPropertyIndex( "anchorPoint" ) );
  DALI_TEST_EQUALS( anchorPoint, Vector2(0.0f,0.0f), TEST_LOCATION );
}

int UtcDaliVisualSetTransform0(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: ColorVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );
  TestTransform( application, visual );
  TestMixColor( visual, ColorVisual::Property::MIX_COLOR, Color::BLUE );

  END_TEST;
}

int UtcDaliVisualSetTransform1(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: PrimitiveVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap[ Visual::Property::TYPE           ] = Visual::PRIMITIVE;
  propertyMap[ PrimitiveVisual::Property::MIX_COLOR ] = Color::WHITE;
  propertyMap[ PrimitiveVisual::Property::SHAPE  ] = PrimitiveVisual::Shape::SPHERE;
  propertyMap[ PrimitiveVisual::Property::SLICES ] = 10;
  propertyMap[ PrimitiveVisual::Property::STACKS ] = 10;
  Visual::Base visual = factory.CreateVisual( propertyMap );
  TestTransform( application, visual );
  TestMixColor( visual, PrimitiveVisual::Property::MIX_COLOR, Color::WHITE );

  END_TEST;
}

int UtcDaliVisualSetTransform2(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: GradientVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE,  Visual::GRADIENT );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR,  Color::GREEN );

  Property::Array stopOffsets;
  stopOffsets.PushBack( 0.0f );
  stopOffsets.PushBack( 0.3f );
  stopOffsets.PushBack( 0.6f );
  stopOffsets.PushBack( 0.8f );
  stopOffsets.PushBack( 1.0f );
  propertyMap.Insert( GradientVisual::Property::STOP_OFFSET, stopOffsets );

  Property::Array stopColors;
  stopColors.PushBack( Vector4( 129.f, 198.f, 193.f, 255.f )/255.f );
  stopColors.PushBack( Vector4( 196.f, 198.f, 71.f, 122.f )/255.f );
  stopColors.PushBack( Vector4( 214.f, 37.f, 139.f, 191.f )/255.f );
  stopColors.PushBack( Vector4( 129.f, 198.f, 193.f, 150.f )/255.f );
  stopColors.PushBack( Color::YELLOW );
  propertyMap.Insert( GradientVisual::Property::STOP_COLOR, stopColors );
  propertyMap.Insert( GradientVisual::Property::CENTER, Vector2( 0.5f, 0.5f ) );
  propertyMap.Insert( GradientVisual::Property::RADIUS, 1.414f );
  Visual::Base visual = factory.CreateVisual( propertyMap );
  TestTransform( application, visual );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::GREEN );

  END_TEST;
}

int UtcDaliVisualSetTransform3(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: BorderVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, Visual::BORDER );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR, Color::MAGENTA );
  propertyMap.Insert( BorderVisual::Property::COLOR, Vector4(0.f, 1.f, 0.f, 0.6f) );
  propertyMap.Insert( BorderVisual::Property::SIZE, 3.0f );
  Visual::Base visual = factory.CreateVisual( propertyMap );
  TestTransform( application, visual );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::MAGENTA );

  END_TEST;
}

int UtcDaliVisualSetTransform4(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: MeshVisual" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, Visual::MESH );
  propertyMap.Insert( DevelVisual::Property::MIX_COLOR, Color::CYAN );

  propertyMap.Insert( "objectUrl", TEST_OBJ_FILE_NAME );
  propertyMap.Insert( "materialUrl", TEST_MTL_FILE_NAME );
  propertyMap.Insert( "texturesPath", TEST_RESOURCE_LOCATION );
  propertyMap.Insert( "shadingMode", MeshVisual::ShadingMode::TEXTURELESS_WITH_DIFFUSE_LIGHTING );
  propertyMap.Insert( "lightPosition", Vector3( 5.0f, 10.0f, 15.0f) );
  Visual::Base visual = factory.CreateVisual( propertyMap );
  TestTransform( application, visual );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::CYAN );

  END_TEST;
}

int UtcDaliVisualSetTransform5(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: ImageVisual from Image" );

  VisualFactory factory = VisualFactory::Get();
  Image image = ResourceImage::New(TEST_IMAGE_FILE_NAME, ImageDimensions(100, 200));
  Visual::Base visual = factory.CreateVisual(image);
  TestTransform( application, visual );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::WHITE );

  END_TEST;
}

int UtcDaliVisualSetTransform6(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: ImageVisual for URL " );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap[Toolkit::Visual::Property::TYPE] = Toolkit::Visual::IMAGE;
  propertyMap[DevelVisual::Property::MIX_COLOR] = Color::YELLOW;
  propertyMap[Toolkit::ImageVisual::Property::URL] = TEST_IMAGE_FILE_NAME;
  propertyMap[Toolkit::ImageVisual::Property::DESIRED_WIDTH] = 100.0f;
  propertyMap[Toolkit::ImageVisual::Property::DESIRED_HEIGHT] = 100.0f;
  propertyMap[Toolkit::ImageVisual::Property::FITTING_MODE] = FittingMode::SCALE_TO_FILL;
  propertyMap[Toolkit::ImageVisual::Property::SAMPLING_MODE] = SamplingMode::BOX_THEN_LINEAR;
  propertyMap[Toolkit::ImageVisual::Property::SYNCHRONOUS_LOADING] = true;
  Visual::Base visual = factory.CreateVisual(propertyMap);
  TestTransform( application, visual );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::YELLOW );

  END_TEST;
}

int UtcDaliVisualSetTransform7(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualSetTransform: NPatch visual" );

  VisualFactory factory = VisualFactory::Get();
  Image image = ResourceImage::New(TEST_NPATCH_FILE_NAME, ImageDimensions(100, 200));
  Visual::Base visual = factory.CreateVisual(image);
  TestTransform( application, visual );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::WHITE );

  END_TEST;
}

int UtcDaliVisualTestTransformPoliciesAsStrings(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualTestTransformPoliciesAsStrings: Use a ColorVisual and test the offset and size policies as strings" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  Property::Map transform;
  transform[ "offsetPolicy" ] = Property::Array().Add( "ABSOLUTE" )
                                                 .Add( "RELATIVE" );
  transform[ "sizePolicy"   ] = Property::Array().Add( "RELATIVE" )
                                                 .Add( "ABSOLUTE" );
  visual.SetTransformAndSize( transform, Vector2(100, 100) );

  Dali::Property::Map visualMap;
  visual.CreatePropertyMap( visualMap );
  Property::Value* value = visualMap.Find( Dali::Toolkit::DevelVisual::Property::TRANSFORM );
  Dali::Property::Map* map = value->GetMap();
  DALI_TEST_CHECK( map );

  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::OFFSET_POLICY );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( typeValue->Get< Vector2 >(), Vector2( Toolkit::DevelVisual::Transform::Policy::ABSOLUTE, Toolkit::DevelVisual::Transform::Policy::RELATIVE ), TEST_LOCATION );
  }
  {
    Property::Value* typeValue = map->Find( Toolkit::DevelVisual::Transform::Property::SIZE_POLICY );
    DALI_TEST_CHECK( typeValue );
    DALI_TEST_EQUALS( typeValue->Get< Vector2 >(), Vector2( Toolkit::DevelVisual::Transform::Policy::RELATIVE, Toolkit::DevelVisual::Transform::Policy::ABSOLUTE ), TEST_LOCATION );
  }

  END_TEST;
}

int UtcDaliNPatchVisualCustomShader(void)
{
  ToolkitTestApplication application;
  tet_infoline( "NPatchVisual with custom shader" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map properties;
  Property::Map shader;
  const std::string fragmentShader = "Foobar";
  shader[Dali::Toolkit::Visual::Shader::Property::FRAGMENT_SHADER] = fragmentShader;

  Property::Map transformMap;
  transformMap["size"] = Vector2( 0.5f, 0.5f ) ;
  transformMap["offset"] = Vector2( 20.0f, 0.0f ) ;
  transformMap["offsetPolicy"] = Vector2( DevelVisual::Transform::Policy::ABSOLUTE, DevelVisual::Transform::Policy::ABSOLUTE );
  transformMap["anchorPoint"] = Align::CENTER;
  transformMap["origin"] = Align::CENTER;
  properties[DevelVisual::Property::TRANSFORM] = transformMap;

  properties[Visual::Property::TYPE] = Visual::IMAGE;
  properties[DevelVisual::Property::MIX_COLOR] = Color::BLUE;
  properties[Visual::Property::SHADER]=shader;
  properties[ImageVisual::Property::URL] = TEST_NPATCH_FILE_NAME;

  Visual::Base visual = factory.CreateVisual( properties );
  TestMixColor( visual, DevelVisual::Property::MIX_COLOR, Color::BLUE );

  // trigger creation through setting on stage
  DummyControl dummy = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(dummy.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );
  dummyImpl.SetLayout( DummyControl::Property::TEST_VISUAL, transformMap );
  dummy.SetSize(2000, 2000);
  dummy.SetParentOrigin(ParentOrigin::CENTER);
  Stage::GetCurrent().Add(dummy);
  application.SendNotification();

  Renderer renderer = dummy.GetRendererAt( 0 );
  Shader shader2 = renderer.GetShader();
  Property::Value value = shader2.GetProperty( Shader::Property::PROGRAM );
  Property::Map* map = value.GetMap();
  DALI_TEST_CHECK( map );

  Property::Index index = renderer.GetPropertyIndex("size");
  DALI_TEST_EQUALS( renderer.GetProperty( index ), Property::Value(Vector2(0.5, 0.5)), 0.001, TEST_LOCATION );

  Property::Value* fragment = map->Find( "fragment" ); // fragment key name from shader-impl.cpp
  // *map["vertex"]; is default here so not verifying it

  DALI_TEST_EQUALS( fragmentShader, fragment->Get<std::string>(), TEST_LOCATION );

  END_TEST;
}

int UtcDaliGradientVisualBlendMode(void)
{
  ToolkitTestApplication application;
  VisualFactory factory = VisualFactory::Get();

  Visual::Base opaqueGradientVisual = factory.CreateVisual(
      Property::Map().Add( Visual::Property::TYPE, Visual::GRADIENT )
                     .Add( GradientVisual::Property::START_POSITION, Vector2( -0.5f, -0.5f ) )
                     .Add( GradientVisual::Property::END_POSITION, Vector2( 0.5f, 0.5f ) )
                     .Add( GradientVisual::Property::STOP_COLOR, Property::Array().Add( Color::RED )
                                                                                  .Add( Color::GREEN ) ) );

  Visual::Base alphaGradientVisual = factory.CreateVisual(
      Property::Map().Add( Visual::Property::TYPE, Visual::GRADIENT )
                     .Add( GradientVisual::Property::START_POSITION, Vector2( -0.5f, -0.5f ) )
                     .Add( GradientVisual::Property::END_POSITION, Vector2( 0.5f, 0.5f ) )
                     .Add( GradientVisual::Property::STOP_COLOR, Property::Array().Add( Color::RED )
                                                                                  .Add( Vector4( 1.0f, 1.0f, 1.0f, 0.5f ) ) ) );

  DummyControl control = DummyControl::New(true);
  control.SetResizePolicy(ResizePolicy::FILL_TO_PARENT, Dimension::ALL_DIMENSIONS);
  Stage::GetCurrent().Add( control );

  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>( control.GetImplementation() );
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL,  opaqueGradientVisual );
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL2, alphaGradientVisual );

  application.SendNotification();
  application.Render();

  // Control should have two renderers, the first one is opaque so our blending mode should be off, the second one has some alpha so should be set to automatic
  DALI_TEST_EQUALS( 2u, control.GetRendererCount(), TEST_LOCATION );
  DALI_TEST_EQUALS( control.GetRendererAt( 0 ).GetProperty( Renderer::Property::BLEND_MODE ).Get<int>(), (int)BlendMode::OFF, TEST_LOCATION );
  DALI_TEST_EQUALS( control.GetRendererAt( 1 ).GetProperty( Renderer::Property::BLEND_MODE ).Get<int>(), (int)BlendMode::AUTO, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualRendererRemovalAndReAddition(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualRendererRemoval" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert(Visual::Property::TYPE,  Visual::COLOR);
  propertyMap.Insert(ColorVisual::Property::MIX_COLOR,  Color::BLUE);
  Visual::Base visual = factory.CreateVisual( propertyMap );

  visual.SetDepthIndex( 1.f );

  DummyControl dummyControl = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(dummyControl.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, visual );
  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 0, TEST_LOCATION );

  dummyControl.SetSize(200.f, 200.f);
  tet_infoline( "Add control with visual to stage and check renderer count is 1" );

  Stage::GetCurrent().Add( dummyControl );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 1, TEST_LOCATION );

  tet_infoline( "Remove control with visual from stage and check renderer count is 0" );
  Stage::GetCurrent().Remove( dummyControl );
  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 0, TEST_LOCATION );

  tet_infoline( "Re-add control with visual to stage and check renderer count is still 1" );

  Stage::GetCurrent().Add( dummyControl );

  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 1, TEST_LOCATION );

  END_TEST;
}



int UtcDaliVisualTextVisualRender(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualTextVisualRender" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::TEXT );
  propertyMap.Insert( "mixColor", Color::WHITE );
  propertyMap.Insert( "renderingBackend", static_cast<int>( Toolkit::Text::DEFAULT_RENDERING_BACKEND ) );
  propertyMap.Insert( "enableMarkup", false );
  propertyMap.Insert( "text", "Hello world" );
  propertyMap.Insert( "fontFamily", "TizenSans" );

  Property::Map fontStyleMapSet;
  fontStyleMapSet.Insert( "weight", "bold" );
  propertyMap.Insert( "fontStyle", fontStyleMapSet );

  propertyMap.Insert( "pointSize", 12.f );
  propertyMap.Insert( "multiLine", true );
  propertyMap.Insert( "horizontalAlignment", "CENTER" );
  propertyMap.Insert( "verticalAlignment", "CENTER" );
  propertyMap.Insert( "textColor", Color::RED );
  Visual::Base textVisual = factory.CreateVisual( propertyMap );
  textVisual.SetDepthIndex( 1.f );

  DummyControl dummyControl = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(dummyControl.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, textVisual );
  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 0, TEST_LOCATION );

  dummyControl.SetSize(200.f, 200.f);
  dummyControl.SetParentOrigin( ParentOrigin::CENTER );

  Stage::GetCurrent().Add( dummyControl );
  application.SendNotification();
  application.Render();


  // Create a texture bigger than the maximum allowed by the image atlas. Used to increase coverage.
  propertyMap.Clear();
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::TEXT );
  propertyMap.Insert( TextVisual::Property::ENABLE_MARKUP, true );
  propertyMap.Insert( TextVisual::Property::TEXT, "<font family='TizenSans' size='12'>Hello world</font>" );
  propertyMap.Insert( TextVisual::Property::MULTI_LINE, true );

  Property::Map transformMap;
  transformMap.Insert( "size", Vector2( 0.5f, 0.5f ) );
  propertyMap.Insert( DevelVisual::Property::TRANSFORM, transformMap );

  textVisual = factory.CreateVisual( propertyMap );
  textVisual.SetDepthIndex( 1.f );

  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, textVisual );
  dummyControl.SetSize( 720.f, 640.f );

  application.SendNotification(); // force process events to ensure text visual
  // adds renderer to the dummy control in OnRelayout
  application.Render();

  Renderer renderer = dummyControl.GetRendererAt(0u);
  Property::Index index = renderer.GetPropertyIndex("size");

  tet_infoline( "Test that the TextVisual has NOT overridden what was set by developer" );
  DALI_TEST_EQUALS( renderer.GetProperty<Vector2>(index), Vector2( 0.5f, 0.5f ), 0.001f, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualTextVisualDisableEnable(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualTextVisualDisableEnable Ensure Text visible can be re-enabled" );

  VisualFactory factory = VisualFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( Visual::Property::TYPE, DevelVisual::TEXT );
  propertyMap.Insert( "mixColor", Color::WHITE );
  propertyMap.Insert( "renderingBackend", static_cast<int>( Toolkit::Text::DEFAULT_RENDERING_BACKEND ) );
  propertyMap.Insert( "enableMarkup", false );
  propertyMap.Insert( "text", "Hello world" );
  propertyMap.Insert( "fontFamily", "TizenSans" );

  Property::Map fontStyleMapSet;
  fontStyleMapSet.Insert( "weight", "bold" );
  propertyMap.Insert( "fontStyle", fontStyleMapSet );

  propertyMap.Insert( "pointSize", 12.f );
  propertyMap.Insert( "multiLine", true );
  propertyMap.Insert( "horizontalAlignment", "CENTER" );
  propertyMap.Insert( "verticalAlignment", "CENTER" );
  propertyMap.Insert( "textColor", Color::RED );
  Visual::Base textVisual = factory.CreateVisual( propertyMap );
  textVisual.SetDepthIndex( 1.f );

  DummyControl dummyControl = DummyControl::New(true);
  Impl::DummyControl& dummyImpl = static_cast<Impl::DummyControl&>(dummyControl.GetImplementation());
  dummyImpl.RegisterVisual( DummyControl::Property::TEST_VISUAL, textVisual );
  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 0, TEST_LOCATION );

  dummyControl.SetSize(200.f, 200.f);
  dummyControl.SetParentOrigin( ParentOrigin::CENTER );

  Stage::GetCurrent().Add( dummyControl );
  application.SendNotification();
  application.Render();

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 1, TEST_LOCATION );

  dummyImpl.EnableVisual( DummyControl::Property::TEST_VISUAL, false );

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 0, TEST_LOCATION );

  dummyImpl.EnableVisual( DummyControl::Property::TEST_VISUAL, true );

  DALI_TEST_EQUALS( dummyControl.GetRendererCount(), 1, TEST_LOCATION );

  END_TEST;
}

int UtcDaliVisualPremultipliedAlpha(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliVisualPremultipliedAlpha" );

  VisualFactory factory = VisualFactory::Get();

  // image visual, test default value ( false )
  {
    Visual::Base imageVisual = factory.CreateVisual(
          Property::Map()
          .Add( Visual::Property::TYPE, Visual::IMAGE )
          .Add( ImageVisual::Property::URL, TEST_IMAGE_FILE_NAME ) );

    Dali::Property::Map visualMap;
    imageVisual.CreatePropertyMap( visualMap );
    Property::Value* value = visualMap.Find( DevelVisual::Property::PREMULTIPLIED_ALPHA );

    // test values
    DALI_TEST_CHECK( value );
    DALI_TEST_EQUALS( value->Get<bool>(), false, TEST_LOCATION );
  }

  // image visual, override premultiplied
  {
    Visual::Base imageVisual = factory.CreateVisual(
          Property::Map()
          .Add( Visual::Property::TYPE, Visual::IMAGE )
          .Add( ImageVisual::Property::URL, TEST_IMAGE_FILE_NAME )
          .Add( DevelVisual::Property::PREMULTIPLIED_ALPHA, true ) );

    Dali::Property::Map visualMap;
    imageVisual.CreatePropertyMap( visualMap );
    Property::Value* value = visualMap.Find( DevelVisual::Property::PREMULTIPLIED_ALPHA );

    // test values
    DALI_TEST_CHECK( value );
    DALI_TEST_EQUALS( value->Get<bool>(), true, TEST_LOCATION);
  }

  // svg visual ( premultiplied alpha by default is true )
  {
    Visual::Base imageVisual = factory.CreateVisual(
          Property::Map()
          .Add( Visual::Property::TYPE, Visual::IMAGE )
          .Add( ImageVisual::Property::URL, TEST_SVG_FILE_NAME ) );

    Dali::Property::Map visualMap;
    imageVisual.CreatePropertyMap( visualMap );
    Property::Value* value = visualMap.Find( DevelVisual::Property::PREMULTIPLIED_ALPHA );

    // test values
    DALI_TEST_CHECK( value );
    DALI_TEST_EQUALS( value->Get<bool>(), true, TEST_LOCATION );
  }

  END_TEST;
}
