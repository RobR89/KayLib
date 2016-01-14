/*
 * Copyright 2016 Robert Reinhart.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef KFONTPROPERTIES_H
#define	KFONTPROPERTIES_H

#include "../IO/KFile.h"

#include <map>
#include <vector>
#include <memory>

namespace KayLib {

class KFontProperties {
private:
  KFontProperties(const KFontProperties& orig) = delete;
  /**
   * Create font properties object from the specified font file.
   * @param font The font file to get the properties for.
   */
  KFontProperties(std::string font);
public:
  virtual ~KFontProperties();

  /**
   * Get the font properties for the specified font file.
   * @param font The font file to get the properties for.
   */
  static std::shared_ptr<KFontProperties> getFontProperties(std::string font);

  /**
   * Remove all font properties from the registry.
   */
  static void clearAllFontProperties();

  /**
   * Find a list of fonts who's names contain fnt
   * @param fnt The string to search for
   * @return A list of found fonts
   */
  static std::vector<std::shared_ptr<KFontProperties>> findFonts(std::string fnt);
  /**
   * Find a list of fonts who's names contain fnt
   * @param fnt The string to search for
   * @param style The style to match. (TTF_STYLE_<NORMAL,BOLD,STRIKETHROUGH,UNDERLINE,ITALIC>)
   * @return A list of found fonts
   */
  static std::vector<std::shared_ptr<KFontProperties>> findFonts(std::string fnt, int style);

  std::string printProperties();

  /**
   * A container for a range of glyphs.
   */
  struct GlyphRange {

    /**
     * Construct an range of 0 to 0.
     */
    GlyphRange() {
      start = end = 0;
    }

    /**
     * Construct a range of nStart to nEnd.
     */
    GlyphRange(int nStart, int nEnd) {
      start = nStart;
      end = nEnd;
    }
    /**
     * The start of the range.
     */
    int start;
    /**
     * The end of the range.
     */
    int end;
  };
  /**
   * The glyph ranges supported by this font.
   */
  std::vector<GlyphRange> glyphRanges;

  /**
   * The path of this font.
   */
  std::string fontPath;
  /**
   * The font family name.
   */
  std::string familyName;
  /**
   * The font style name.
   */
  std::string styleName;
  /**
   * The TTF_Style of the font.
   */
  int ttf_Style;
  /**
   * The height of the font.
   */
  int height;
  /**
   * The ascent of the font above the baseline.
   */
  int ascent;
  /**
   * The descent of the font below the baseline.
   */
  int descent;
  /**
   * The spacing between lines of text.
   */
  int lineSkip;
  /**
   * Is the font a fixed width font.
   */
  bool monospace;
  /**
   * Number of faces in the font.
   */
  int faces;

  /**
   * The first available glyph in the font.
   */
  int firstGlyph;
  /**
   * The last available glyph in the font.
   */
  int lastGlyph;
  /**
   * The total number of glyphs in the font.
   */
  int totalGlyphs;

  /**
   * Enumerate fonts from all directories registered with KApplication::AddFontPath recursing to a depth of depth.
   * @param depth the depth of the directory recursion.
   */
  static void enumerateFonts(int depth);
private:
  /**
   * Enumerate a specific file.
   * @param file The file to enumerate.
   * @return True if file was a ttf file.
   */
  static bool enumerateFont(const KayLib::KFile *file);
  /**
   * Enumerate fonts from the directory recursing to a depth of depth.
   * @param sPath the directory to enumerate.
   * @param depth the depth of the directory recursion.
   */
  static void enumerateDirectory(const std::string sPath, int depth);

public:
  //http://www.russellcottrell.com/greek/utilities/UnicodeRanges.htm
  // to-do: add more glyph ranges.

  // European.

  /**
   * Returns range of Latin glyphs. 128 possible
   */
  static KFontProperties::GlyphRange range_Latin1() {
    return GlyphRange(0x0000, 0x07f);
  };

  /**
   * Returns range of Greek glyphs. 144 possible
   */
  static KFontProperties::GlyphRange range_Greek() {
    return GlyphRange(0x0370, 0x03ff);
  };

  /**
   * Returns the range of Cyrillic glyphs. 304 possible
   */
  static KFontProperties::GlyphRange range_Cyrillic() {
    return GlyphRange(0x0400, 0x052f);
  };

  /**
   * Returns the range of Armenian glyphs. 96 possible
   */
  static KFontProperties::GlyphRange range_Armenian() {
    return GlyphRange(0x0530, 0x058f);
  };

  /**
   * Returns the range of Georgian glyphs. 96 possible
   */
  static KFontProperties::GlyphRange range_Georgian() {
    return GlyphRange(0x10a0, 0x10ff);
  };

  /**
   * Returns the range of Runic glyphs. 96 possible
   */
  static KFontProperties::GlyphRange range_Runic() {
    return GlyphRange(0x16a0, 0x16ff);
  };

  /**
   * Returns the range of Coptic glyphs. 128 possible
   */
  static KFontProperties::GlyphRange range_Coptic() {
    return GlyphRange(0x2c80, 0x2cff);
  };

  // East Asian

  /**
   * Returns range of CJK (Chinese, Japanese, Korean) glyphs. 20944 possible
   */
  static KFontProperties::GlyphRange range_CJK() {
    return GlyphRange(0x4e00, 0x9fcf);
  };

  /**
   * Returns the range of hiragana glyphs. 96 possible
   */
  static KFontProperties::GlyphRange range_Hiragana() {
    return GlyphRange(0x3040, 0x309f);
  };

  /**
   * Returns the range of Katakana glyphs. 96 possible
   */
  static KFontProperties::GlyphRange range_Katakana() {
    return GlyphRange(0x30a0, 0x30ff);
  };

  /**
   * Count the number of glyphs that exist in the specified range for this font.
   * @param range the range to search.
   */
  int glyphsInRange(const KFontProperties::GlyphRange range);

};

}

#endif	/* KFONTPROPERTIES_H */

