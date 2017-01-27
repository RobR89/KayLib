/*
 * Copyright 2017 Robert Reinhart.
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
#define KFONTPROPERTIES_H

#include "../IO/KFile.h"

#include <vector>
#include <memory>
// include SDL headers
#include <SDL2/SDL_ttf.h>

namespace KayLib
{

    class KFontProperties
    {
    private:
        KFontProperties(const KFontProperties& orig);
        /**
         * Create font properties object from the specified font file.
         * @param font The font file to get the properties for.
         * @param ptSize The point size of the font.
         */
        KFontProperties(const TTF_Font *_Font, const KFile *file, int ptSize);

    public:
        virtual ~KFontProperties();

        /**
         * Get the font properties for the specified font file.
         * @param fontFile The font file to get the properties for.
         * @param ptSize The point size of the font.
         */
        static std::shared_ptr<KFontProperties> create(const std::string &fontFile, int ptSize);

        /**
         * Get the font properties for the specified font file.
         * @param fontFile The font file to get the properties for.
         * @param ptSize The point size of the font.
         */
        static std::shared_ptr<KFontProperties> create(const KFile &file, int ptSize);

        /**
         * Remove all font properties from the registry.
         */
        static void clearGlobalFontProperties();

        /**
         * Find a list of fonts who's names contain fnt
         * @param fnt The string to search for
         * @return A list of found fonts
         */
        static std::vector<std::shared_ptr<KFontProperties>> findFonts(const std::string &fnt);
        /**
         * Find a list of fonts who's names contain fnt
         * @param fnt The string to search for
         * @param style The style to match. (TTF_STYLE_<NORMAL,BOLD,STRIKETHROUGH,UNDERLINE,ITALIC>)
         * @return A list of found fonts
         */
        static std::vector<std::shared_ptr<KFontProperties>> findFonts(const std::string &fnt, int style);

        /**
         * Print the font properties.
         * @return The string containing the font properties.
         */
        std::string printProperties() const;

        /**
         * A container for a range of glyphs.
         */
        struct GlyphRange
        {

            /**
             * Construct a range of nStart to nEnd.
             */
            GlyphRange(int nStart, int nEnd) : start(nStart), end(nEnd) { }
            /**
             * The start of the range.
             */
            const int start;
            /**
             * The end of the range.
             */
            const int end;
        };

        /**
         * The glyph ranges supported by this font.
         */
        const std::vector<GlyphRange> glyphRanges;

        /**
         * The path of this font.
         */
        const std::string fontPath;
        /**
         * The font family name.
         */
        const std::string familyName;
        /**
         * The font style name.
         */
        const std::string styleName;
        /**
         * The TTF_Style of the font.
         */
        const int ttf_Style;
        /**
         * The height of the font.
         */
        const int height;
        /**
         * The ascent of the font above the baseline.
         */
        const int ascent;
        /**
         * The descent of the font below the baseline.
         */
        const int descent;
        /**
         * The spacing between lines of text.
         */
        const int lineSkip;
        /**
         * Is the font a fixed width font.
         */
        const bool monospace;
        /**
         * Number of faces in the font.
         */
        const int faces;

        /**
         * The first available glyph in the font.
         */
        const int firstGlyph;
        /**
         * The last available glyph in the font.
         */
        const int lastGlyph;
        /**
         * The total number of glyphs in the font.
         */
        const int totalGlyphs;
        /**
         * Point size of the font.
         */
        const int pointSize;

        /**
         * Enumerate fonts from all directories registered with KApplication::AddFontPath recursing to a depth of depth.
         * @param depth the depth of the directory recursion.
         * @param ptSize The point size of the fonts to enumerate.
         */
        static void enumerateFonts(int depth, int ptSize);
    private:
        /**
         * Enumerate a specific file.
         * @param file The file to enumerate.
         * @param ptSize The point size of the font.
         * @return True if file was a ttf file.
         */
        static bool enumerateFont(const KayLib::KFile *file, int ptSize);
        /**
         * Enumerate fonts from the directory recursing to a depth of depth.
         * @param sPath the directory to enumerate.
         * @param depth the depth of the directory recursion.
         * @param ptSize The point size of the fonts to enumerate.
         */
        static void enumerateDirectory(const KFile &sPath, int depth, int ptSize);

    public:
        //http://www.russellcottrell.com/greek/utilities/UnicodeRanges.htm
        // to-do: add more glyph ranges.

        // European.

        /**
         * Returns range of Latin glyphs. 128 possible
         */
        static GlyphRange range_Latin1()
        {
            return GlyphRange(0x0000, 0x07f);
        };

        /**
         * Returns range of Greek glyphs. 144 possible
         */
        static GlyphRange range_Greek()
        {
            return GlyphRange(0x0370, 0x03ff);
        };

        /**
         * Returns the range of Cyrillic glyphs. 304 possible
         */
        static GlyphRange range_Cyrillic()
        {
            return GlyphRange(0x0400, 0x052f);
        };

        /**
         * Returns the range of Armenian glyphs. 96 possible
         */
        static GlyphRange range_Armenian()
        {
            return GlyphRange(0x0530, 0x058f);
        };

        /**
         * Returns the range of Georgian glyphs. 96 possible
         */
        static GlyphRange range_Georgian()
        {
            return GlyphRange(0x10a0, 0x10ff);
        };

        /**
         * Returns the range of Runic glyphs. 96 possible
         */
        static GlyphRange range_Runic()
        {
            return GlyphRange(0x16a0, 0x16ff);
        };

        /**
         * Returns the range of Coptic glyphs. 128 possible
         */
        static GlyphRange range_Coptic()
        {
            return GlyphRange(0x2c80, 0x2cff);
        };

        // East Asian

        /**
         * Returns range of CJK (Chinese, Japanese, Korean) glyphs. 20944 possible
         */
        static GlyphRange range_CJK()
        {
            return GlyphRange(0x4e00, 0x9fcf);
        };

        /**
         * Returns the range of hiragana glyphs. 96 possible
         */
        static GlyphRange range_Hiragana()
        {
            return GlyphRange(0x3040, 0x309f);
        };

        /**
         * Returns the range of Katakana glyphs. 96 possible
         */
        static GlyphRange range_Katakana()
        {
            return GlyphRange(0x30a0, 0x30ff);
        };

        /**
         * Count the number of glyphs that exist in the specified range for this font.
         * @param range the range to search.
         */
        int glyphsInRange(const GlyphRange &range) const;

    };

}

#endif /* KFONTPROPERTIES_H */

