#ifndef __DALI_TOOLKIT_TEXT_LINE_RUN_H__
#define __DALI_TOOLKIT_TEXT_LINE_RUN_H__

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

// EXTERNAL INCLUDES
#include <dali/public-api/math/vector2.h>

// INTERNAL INCLUDES
#include <dali-toolkit/internal/text/character-run.h>

namespace Dali
{

namespace Toolkit
{

namespace Text
{

/**
 * @brief LineRun
 */
struct LineRun
{
  GlyphIndex         glyphIndex;      ///< The initial glyph index.
  Length             numberOfGlyphs;  ///< The number of glyphs of the run.
  CharacterRun       characterRun;    ///< The initial character and the number of characters.
  float              width;           ///< The line's width.
  float              ascender;        ///< The line's ascender.
  float              descender;       ///< The line's descender.
  float              extraLength;     ///< The length of the white spaces at the end of the line.
  float              alignmentOffset; ///< The horizontal alignment offset.
  CharacterDirection direction : 1;   ///< Direction of the first character of the paragraph.
  bool               ellipsis  : 1;   ///< Wheter ellipsis is added to the line.
};

} // namespace Text

} // namespace Toolkit

} // namespace Dali

#endif // __DALI_TOOLKIT_TEXT_LINE_RUN_H__
