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

#include "radial-gradient.h"

namespace Dali
{

namespace Toolkit
{

namespace Internal
{

RadialGradient::RadialGradient( const Vector2& center, float radius )
: Gradient()
{
  SetCenterAndRadius( center, radius );
}

RadialGradient::~RadialGradient()
{}

void RadialGradient::SetCenterAndRadius( const Vector2& center, float radius )
{
  mCenter = center;
  mRadius = radius;

  // Calculate the transform aligning to the circle
  Matrix3 alignMatrix( mRadius, 0.f, 0.f, 0.f, mRadius, 0.f, mCenter.x, mCenter.y, 1.f );
  alignMatrix.Invert();

  mAlignmentTransform = alignMatrix;
}

const Vector2& RadialGradient::GetCenter() const
{
  return mCenter;
}

float RadialGradient::GetRadius() const
{
  return mRadius;
}

} // namespace Internal

} // namespace Toolkit

} // namespace Dali
