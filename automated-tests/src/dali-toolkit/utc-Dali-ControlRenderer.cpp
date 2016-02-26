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

#include <iostream>
#include <stdlib.h>
#include <dali-toolkit-test-suite-utils.h>
#include <dali/devel-api/rendering/renderer.h>
#include <dali/devel-api/rendering/material.h>
#include <dali/devel-api/rendering/shader.h>
#include <dali-toolkit/devel-api/controls/renderer-factory/renderer-factory.h>

using namespace Dali;
using namespace Dali::Toolkit;

namespace
{
const char* TEST_IMAGE_FILE_NAME =  "gallery_image_01.jpg";
const char* TEST_NPATCH_FILE_NAME =  "gallery_image_01.9.jpg";
}

void dali_control_renderer_startup(void)
{
  test_return_value = TET_UNDEF;
}

void dali_control_renderer_cleanup(void)
{
  test_return_value = TET_PASS;
}

int UtcDaliControlRendererCopyAndAssignment(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererCopyAndAssignment" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "colorRenderer");
  propertyMap.Insert("blendColor",  Color::BLUE);
  ControlRenderer controlRenderer = factory.GetControlRenderer( propertyMap );

  ControlRenderer controlRendererCopy( controlRenderer );
  DALI_TEST_CHECK(controlRenderer == controlRendererCopy);

  ControlRenderer emptyControlRenderer;
  ControlRenderer emptyControlRendererCopy( emptyControlRenderer );
  DALI_TEST_CHECK(emptyControlRenderer == emptyControlRendererCopy);

  ControlRenderer controlRendererEquals;
  controlRendererEquals = controlRenderer;
  DALI_TEST_CHECK(controlRenderer == controlRendererEquals);

  ControlRenderer emptyControlRendererEquals;
  emptyControlRendererEquals = emptyControlRenderer;
  DALI_TEST_CHECK( emptyControlRenderer == emptyControlRendererEquals );

  //self assignment
  controlRenderer = controlRenderer;
  DALI_TEST_CHECK( controlRenderer = controlRendererCopy );

  END_TEST;
}

int UtcDaliControlRendererSetGetDepthIndex(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererSetDepthIndex" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "colorRenderer");
  propertyMap.Insert("blendColor",  Color::BLUE);
  ControlRenderer controlRenderer = factory.GetControlRenderer( propertyMap );

  controlRenderer.SetDepthIndex( 1.f );

  Actor actor = Actor::New();
  actor.SetSize(200.f, 200.f);
  Stage::GetCurrent().Add( actor );
  controlRenderer.SetOnStage( actor );

  int depthIndex = actor.GetRendererAt(0u).GetProperty<int>( Renderer::Property::DEPTH_INDEX );
  DALI_TEST_EQUALS( depthIndex, 1, TEST_LOCATION );
  DALI_TEST_EQUALS( controlRenderer.GetDepthIndex(), 1.f, TEST_LOCATION );

  controlRenderer.SetDepthIndex( -1.f );
  depthIndex = actor.GetRendererAt(0u).GetProperty<int>( Renderer::Property::DEPTH_INDEX );
  DALI_TEST_EQUALS( depthIndex, -1, TEST_LOCATION );
  DALI_TEST_EQUALS( controlRenderer.GetDepthIndex(), -1.f, TEST_LOCATION );

  END_TEST;
}

int UtcDaliControlRendererSize(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetNaturalSize" );

  RendererFactory factory = RendererFactory::Get();
  Vector2 rendererSize( 20.f, 30.f );
  Vector2 naturalSize;

  // color renderer
  ControlRenderer colorRenderer = factory.GetControlRenderer( Color::MAGENTA );
  colorRenderer.SetSize( rendererSize );
  DALI_TEST_EQUALS( colorRenderer.GetSize(), rendererSize, TEST_LOCATION );
  colorRenderer.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2::ZERO, TEST_LOCATION );

  // image renderer
  Image image = ResourceImage::New(TEST_IMAGE_FILE_NAME, ImageDimensions(100, 200));
  ControlRenderer imageRenderer = factory.GetControlRenderer( image );
  imageRenderer.SetSize( rendererSize );
  DALI_TEST_EQUALS( imageRenderer.GetSize(), rendererSize, TEST_LOCATION );
  imageRenderer.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2(100.f, 200.f), TEST_LOCATION );

  // n patch renderer
  TestPlatformAbstraction& platform = application.GetPlatform();
  Vector2 testSize(80.f, 160.f);
  platform.SetClosestImageSize(testSize);
  image = ResourceImage::New(TEST_NPATCH_FILE_NAME);
  ControlRenderer nPatchRenderer = factory.GetControlRenderer( image );
  nPatchRenderer.SetSize( rendererSize );
  DALI_TEST_EQUALS( nPatchRenderer.GetSize(), rendererSize, TEST_LOCATION );
  nPatchRenderer.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, testSize, TEST_LOCATION );

  // border renderer
  float borderSize = 5.f;
  ControlRenderer borderRenderer = factory.GetControlRenderer( borderSize, Color::RED );
  borderRenderer.SetSize( rendererSize );
  DALI_TEST_EQUALS( borderRenderer.GetSize(), rendererSize, TEST_LOCATION );
  borderRenderer.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2::ZERO, TEST_LOCATION );

  // gradient renderer
  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "gradientRenderer");
  Vector2 start(-1.f, -1.f);
  Vector2 end(1.f, 1.f);
  propertyMap.Insert("gradientStartPosition",   start);
  propertyMap.Insert("gradientEndPosition",   end);
  propertyMap.Insert("gradientStopOffset",   Vector2(0.f, 1.f));
  Property::Array stopColors;
  stopColors.PushBack( Color::RED );
  stopColors.PushBack( Color::GREEN );
  propertyMap.Insert("gradientStopColor",   stopColors);
  ControlRenderer gradientRenderer = factory.GetControlRenderer(propertyMap);
  gradientRenderer.SetSize( rendererSize );
  DALI_TEST_EQUALS( gradientRenderer.GetSize(), rendererSize, TEST_LOCATION );
  gradientRenderer.GetNaturalSize(naturalSize);
  DALI_TEST_EQUALS( naturalSize, Vector2::ZERO,TEST_LOCATION );

  END_TEST;
}

int UtcDaliControlRendererSetOnOffStage(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererSetDepthIndex" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "colorRenderer");
  propertyMap.Insert("blendColor",  Color::BLUE);
  ControlRenderer controlRenderer = factory.GetControlRenderer( propertyMap );

  Actor actor = Actor::New();
  actor.SetSize(200.f, 200.f);
  Stage::GetCurrent().Add( actor );

  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 0u );

  controlRenderer.SetOnStage( actor );
  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 1u );

  controlRenderer.SetOffStage( actor );
  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 0u );

  END_TEST;
}

int UtcDaliControlRendererRemoveAndReset(void)
{
  ToolkitTestApplication application;
  tet_infoline( "intUtcDaliControlRendererRemoveAndReset" );

  RendererFactory factory = RendererFactory::Get();

  Actor actor = Actor::New();
  actor.SetSize(200.f, 200.f);
  Stage::GetCurrent().Add( actor );

  ControlRenderer imageRenderer;
  // test calling RemoveAndReset with an empty handle
  try
  {
    imageRenderer.RemoveAndReset( actor );
    tet_result(TET_PASS);
  }
  catch (DaliException& exception)
  {
    tet_result(TET_FAIL);
  }

  Image image = ResourceImage::New(TEST_IMAGE_FILE_NAME, ImageDimensions(100, 200));
  imageRenderer = factory.GetControlRenderer(image);
  DALI_TEST_CHECK( imageRenderer );

  imageRenderer.SetOnStage( actor );
  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 1u );

  imageRenderer.RemoveAndReset( actor );
  application.SendNotification();
  application.Render(0);
  DALI_TEST_CHECK( actor.GetRendererCount() == 0u ); // renderer is removed from actor
  DALI_TEST_CHECK( !imageRenderer ); // control renderer is reset

  END_TEST;
}

int UtcDaliControlRendererGetPropertyMap1(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetPropertyMap1: ColorRenderer" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "colorRenderer");
  propertyMap.Insert("blendColor",  Color::BLUE);
  ControlRenderer colorRenderer = factory.GetControlRenderer( propertyMap );

  Property::Map resultMap;
  colorRenderer.CreatePropertyMap( resultMap );

  Property::Value* typeValue = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( typeValue );
  DALI_TEST_CHECK( typeValue->Get<std::string>() == "colorRenderer" );

  Property::Value* colorValue = resultMap.Find( "blendColor",  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::BLUE );

  // change the blend color
  Actor actor;
  factory.ResetRenderer( colorRenderer, actor, Color::CYAN );
  colorRenderer.CreatePropertyMap( resultMap );

  colorValue = resultMap.Find( "blendColor",  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::CYAN );

  END_TEST;
}

int UtcDaliControlRendererGetPropertyMap2(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetPropertyMap2: BorderRenderer" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "borderRenderer");
  propertyMap.Insert("borderColor",  Color::BLUE);
  propertyMap.Insert("borderSize",  5.f);
  ControlRenderer borderRenderer = factory.GetControlRenderer( propertyMap );

  Property::Map resultMap;
  borderRenderer.CreatePropertyMap( resultMap );

  // check the property values from the returned map from control renderer
  Property::Value* typeValue = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( typeValue );
  DALI_TEST_CHECK( typeValue->Get<std::string>() == "borderRenderer" );

  Property::Value* colorValue = resultMap.Find( "borderColor",  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::BLUE );

  Property::Value* sizeValue = resultMap.Find( "borderSize",  Property::FLOAT );
  DALI_TEST_CHECK( sizeValue );
  DALI_TEST_CHECK( sizeValue->Get<float>() == 5.f );

  borderRenderer = factory.GetControlRenderer( 10.f, Color::CYAN );
  borderRenderer.CreatePropertyMap( resultMap );

  typeValue = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( typeValue );
  DALI_TEST_CHECK( typeValue->Get<std::string>() == "borderRenderer" );

   colorValue = resultMap.Find( "borderColor",  Property::VECTOR4 );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<Vector4>() == Color::CYAN );

  colorValue = resultMap.Find( "borderSize",  Property::FLOAT );
  DALI_TEST_CHECK( colorValue );
  DALI_TEST_CHECK( colorValue->Get<float>() == 10.f );

  END_TEST;
}

int UtcDaliControlRendererGetPropertyMap3(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetPropertyMap3: linear GradientRenderer" );

  RendererFactory factory = RendererFactory::Get();
  DALI_TEST_CHECK( factory );

  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "gradientRenderer");

  Vector2 start(-1.f, -1.f);
  Vector2 end(1.f, 1.f);
  propertyMap.Insert("gradientStartPosition",   start);
  propertyMap.Insert("gradientEndPosition",   end);
  propertyMap.Insert("gradientSpreadMethod",   "repeat");

  propertyMap.Insert("gradientStopOffset",   Vector2(0.2f, 0.8f));

  Property::Array stopColors;
  stopColors.PushBack( Color::RED );
  stopColors.PushBack( Color::GREEN );
  propertyMap.Insert("gradientStopColor",   stopColors);

  ControlRenderer gradientRenderer = factory.GetControlRenderer(propertyMap);

  Property::Map resultMap;
  gradientRenderer.CreatePropertyMap( resultMap );

  // check the property values from the returned map from control renderer
  Property::Value* value = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "gradientRenderer" );

  value = resultMap.Find( "gradientUnits",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "objectBoundingBox" );

  value = resultMap.Find( "gradientSpreadMethod",   Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "repeat" );

  value = resultMap.Find( "gradientStartPosition",   Property::VECTOR2 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector2>(), start , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( "gradientEndPosition",   Property::VECTOR2 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector2>(), end , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( "gradientStopOffset",   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* offsetArray = value->GetArray();
  DALI_TEST_CHECK( offsetArray->Count() == 2 );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(0).Get<float>(), 0.2f , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(1).Get<float>(), 0.8f , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( "gradientStopColor",   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* colorArray = value->GetArray();
  DALI_TEST_CHECK( colorArray->Count() == 2 );
  DALI_TEST_EQUALS( colorArray->GetElementAt(0).Get<Vector4>(), Color::RED , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( colorArray->GetElementAt(1).Get<Vector4>(), Color::GREEN , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  END_TEST;
}

int UtcDaliControlRendererGetPropertyMap4(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetPropertyMap4: radial GradientRenderer" );

  RendererFactory factory = RendererFactory::Get();
  DALI_TEST_CHECK( factory );

  Property::Map propertyMap;
  propertyMap.Insert("rendererType",  "gradientRenderer");

  Vector2 center(100.f, 100.f);
  float radius = 100.f;
  propertyMap.Insert("gradientUnits",  "userSpace");
  propertyMap.Insert("gradientCenter",  center);
  propertyMap.Insert("gradientRadius",  radius);
  propertyMap.Insert("gradientStopOffset",   Vector3(0.1f, 0.3f, 1.1f));

  Property::Array stopColors;
  stopColors.PushBack( Color::RED );
  stopColors.PushBack( Color::BLACK );
  stopColors.PushBack( Color::GREEN );
  propertyMap.Insert("gradientStopColor",   stopColors);

  ControlRenderer gradientRenderer = factory.GetControlRenderer(propertyMap);
  DALI_TEST_CHECK( gradientRenderer );

  Property::Map resultMap;
  gradientRenderer.CreatePropertyMap( resultMap );

  // check the property values from the returned map from control renderer
  Property::Value* value = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "gradientRenderer" );

  value = resultMap.Find( "gradientUnits",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "userSpace" );

  value = resultMap.Find( "gradientSpreadMethod",   Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "pad" );

  value = resultMap.Find( "gradientCenter",  Property::VECTOR2 );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<Vector2>(), center , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( "gradientRadius",  Property::FLOAT );
  DALI_TEST_CHECK( value );
  DALI_TEST_EQUALS( value->Get<float>(), radius , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( "gradientStopOffset",   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* offsetArray = value->GetArray();
  DALI_TEST_CHECK( offsetArray->Count() == 3 );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(0).Get<float>(), 0.1f , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( offsetArray->GetElementAt(1).Get<float>(), 0.3f , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  // any stop value will be clamped to [0.0, 1.0];
  DALI_TEST_EQUALS( offsetArray->GetElementAt(2).Get<float>(), 1.0f , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  value = resultMap.Find( "gradientStopColor",   Property::ARRAY );
  DALI_TEST_CHECK( value );
  Property::Array* colorArray = value->GetArray();
  DALI_TEST_CHECK( colorArray->Count() == 3 );
  DALI_TEST_EQUALS( colorArray->GetElementAt(0).Get<Vector4>(), Color::RED , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( colorArray->GetElementAt(1).Get<Vector4>(), Color::BLACK , Math::MACHINE_EPSILON_100, TEST_LOCATION );
  DALI_TEST_EQUALS( colorArray->GetElementAt(2).Get<Vector4>(), Color::GREEN , Math::MACHINE_EPSILON_100, TEST_LOCATION );

  END_TEST;
}

int UtcDaliControlRendererGetPropertyMap5(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetPropertyMap5: ImageRenderer" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( "rendererType",  "imageRenderer" );
  propertyMap.Insert( "imageUrl",  TEST_IMAGE_FILE_NAME );
  propertyMap.Insert( "imageDesiredWidth",   20 );
  propertyMap.Insert( "imageDesiredHeight",   30 );
  propertyMap.Insert( "imageFittingMode",   "fitHeight" );
  propertyMap.Insert( "imageSamplingMode",   "boxThenNearest" );

  ControlRenderer imageRenderer = factory.GetControlRenderer(propertyMap);
  DALI_TEST_CHECK( imageRenderer );

  Property::Map resultMap;
  imageRenderer.CreatePropertyMap( resultMap );

  // check the property values from the returned map from control renderer
  Property::Value* value = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "imageRenderer" );

  value = resultMap.Find( "imageUrl",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_IMAGE_FILE_NAME );

  value = resultMap.Find( "imageFittingMode",   Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "fitHeight" );

  value = resultMap.Find( "imageSamplingMode",   Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "boxThenNearest" );

  value = resultMap.Find( "imageDesiredWidth",   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 20 );

  value = resultMap.Find( "imageDesiredHeight",   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 30 );

  // Get an image renderer with an image handle, and test the default property values
  Image image = ResourceImage::New(TEST_IMAGE_FILE_NAME, ImageDimensions(100, 200));
  imageRenderer = factory.GetControlRenderer(image);
  imageRenderer.CreatePropertyMap( resultMap );

  value = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "imageRenderer" );

  value = resultMap.Find( "imageUrl",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_IMAGE_FILE_NAME );

  value = resultMap.Find( "imageFittingMode",   Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "shrinkToFit" );

  value = resultMap.Find( "imageSamplingMode",   Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "box" );

  value = resultMap.Find( "imageDesiredWidth",   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 100 );

  value = resultMap.Find( "imageDesiredHeight",   Property::INTEGER );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<int>() == 200 );

  END_TEST;
}

int UtcDaliControlRendererGetPropertyMap6(void)
{
  ToolkitTestApplication application;
  tet_infoline( "UtcDaliControlRendererGetPropertyMap6: NPatchRenderer" );

  RendererFactory factory = RendererFactory::Get();
  Property::Map propertyMap;
  propertyMap.Insert( "rendererType",  "nPatchRenderer" );
  propertyMap.Insert( "imageUrl",  TEST_NPATCH_FILE_NAME );
  propertyMap.Insert( "borderOnly",  true );
  ControlRenderer nPatchRenderer = factory.GetControlRenderer( propertyMap );

  Property::Map resultMap;
  nPatchRenderer.CreatePropertyMap( resultMap );

  // check the property values from the returned map from control renderer
  Property::Value* value = resultMap.Find( "rendererType",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == "nPatchRenderer" );

  value = resultMap.Find( "imageUrl",  Property::STRING );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<std::string>() == TEST_NPATCH_FILE_NAME );

  value = resultMap.Find( "borderOnly",  Property::BOOLEAN );
  DALI_TEST_CHECK( value );
  DALI_TEST_CHECK( value->Get<bool>() );

  END_TEST;
}
