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
#include "../String/KString.h"

#include <memory>
#include <sstream>
#include <mutex>
#include <map>
#include <vector>
// include SDL headers
#include <SDL2/SDL_ttf.h>

namespace KayLib
{

    class KFontProperties
    {
    private:

        KFontProperties(const KFontProperties& orig) :
        fontPath(orig.fontPath),
        familyName(orig.familyName),
        styleName(orig.styleName),
        ttf_Style(orig.ttf_Style),
        height(orig.height),
        ascent(orig.ascent),
        descent(orig.descent),
        lineSkip(orig.lineSkip),
        faces(orig.faces),
        monospace(orig.monospace),
        firstGlyph(orig.firstGlyph),
        lastGlyph(orig.lastGlyph),
        totalGlyphs(orig.totalGlyphs),
        glyphRanges(orig.glyphRanges),
        pointSize(orig.pointSize) { }

        /**
         * Create font properties object from the specified font file.
         * @param font The font file the properties are for.
         * @param ptSize The point size of the font.
         */
        KFontProperties(const TTF_Font *_Font, const KFile *file, int ptSize) :
        fontPath(file->getAbsolutePath().c_str()),
        familyName(TTF_FontFaceFamilyName(_Font)),
        styleName(TTF_FontFaceStyleName(_Font)),
        ttf_Style(TTF_GetFontStyle(_Font)),
        height(TTF_FontHeight(_Font)),
        ascent(TTF_FontAscent(_Font)),
        descent(TTF_FontDescent(_Font)),
        lineSkip(TTF_FontLineSkip(_Font)),
        faces(TTF_FontFaces(_Font)),
        monospace(TTF_FontFaceIsFixedWidth(_Font)),
        firstGlyph(getFirstGlyph(_Font)),
        lastGlyph(getLastGlyph(_Font)),
        totalGlyphs(getTotalGlyphs(_Font)),
        glyphRanges(getRanges(_Font)),
        pointSize(ptSize) { }

    public:
        virtual ~KFontProperties()
        {
        }

        /**
         * Get the font properties for the specified font file.
         * @param fontFile The font file to get the properties for.
         * @param ptSize The point size of the font.
         */
        static std::shared_ptr<KFontProperties> create(const std::string &fontFile, int ptSize)
        {
            KFile file(fontFile);
            return create(file, ptSize);
        }

        /**
         * Get the font properties for the specified font file.
         * @param fontFile The font file to get the properties for.
         * @param ptSize The point size of the font.
         */
        static std::shared_ptr<KFontProperties> create(const KFile &file, int ptSize)
        {
            std::string absFont = file.getAbsolutePath();
            auto lock = getLock();
            fontList_t &fontList = getFontList();
            if(fontList.find(absFont) != fontList.end())
            {
                if(fontList[absFont].find(ptSize) != fontList[absFont].end())
                {
                    return fontList[absFont][ptSize];
                }
            }
            lock.unlock();
            if(!KFontProperties::enumerateFont(&file, ptSize))
            {
                return std::shared_ptr<KFontProperties>();
            }
            lock.lock();
            return fontList[absFont][ptSize];
        }

        /**
         * Remove all font properties from the registry.
         */
        static void clearGlobalFontProperties()
        {
            auto lock = getLock();
            fontList_t &fontList = getFontList();
            fontList.clear();
        }

        /**
         * Find a list of fonts who's names contain fnt
         * @param fnt The string to search for
         * @return A list of found fonts
         */
        static std::vector<std::shared_ptr<KFontProperties>> findFonts(const std::string &fnt)
        {
            std::vector<std::shared_ptr < KFontProperties>> found;
            auto lock = getLock();
            fontList_t &fontList = getFontList();
            std::string find = KString::strToLower(fnt);
            for(auto itr : fontList)
            {
                auto absPath = itr.first;
                absPath = KString::strToLower(absPath);
                if(absPath.find(find) != std::string::npos)
                {
                    for(auto points : itr.second)
                    {
                        std::shared_ptr<KFontProperties> fp = points.second;
                        found.push_back(fp);
                    }
                }
            }
            return found;
        }

        /**
         * Find a list of fonts who's names contain fnt
         * @param fnt The string to search for
         * @param style The style to match. (TTF_STYLE_<NORMAL,BOLD,STRIKETHROUGH,UNDERLINE,ITALIC>)
         * @return A list of found fonts
         */
        static std::vector<std::shared_ptr<KFontProperties>> findFonts(const std::string &fnt, int style)
        {
            std::vector<std::shared_ptr < KFontProperties>> found = findFonts(fnt);
            auto fp = found.begin();
            while(fp != found.end())
            {
                if((*fp)->ttf_Style != style)
                {
                    fp = found.erase(fp);
                }
                else
                {
                    fp++;
                }
            }
            return found;
        }

        /**
         * Print the font properties.
         * @return The string containing the font properties.
         */
        std::string printProperties() const
        {
            std::ostringstream ss;
            ss << "path: " << fontPath << std::endl;
            ss << "family name: " << familyName << std::endl;
            ss << "style name:  " << styleName << std::endl;
            ss << "Fixed width: " << monospace << std::endl;
            return ss.str();
        }

        /**
         * Print the font properties.
         * @param padding A string to prepend to the beginning of each line
         * @return The string containing the font properties.
         */
        std::string printProperties(const std::string padding) const
        {
            std::ostringstream ss;
            ss << padding << "path: " << fontPath << std::endl;
            ss << padding << "family name: " << familyName << std::endl;
            ss << padding << "style name:  " << styleName << std::endl;
            ss << padding << "Fixed width: " << monospace << std::endl;
            return ss.str();
        }

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
         * @param paths The List of paths to search can be returned from KFont::getFontPaths();
         * @param depth The depth of the directory recursion.
         * @param ptSize The point size of the fonts to enumerate.
         */
        static void enumerateFonts(const std::vector<KFile> &paths, int depth, int ptSize)
        {
            std::string found;
            for(auto sPath : paths)
            {
                enumerateDirectory(sPath, depth, ptSize);
            }
        }

        static int countFonts()
        {
            fontList_t &fontList = getFontList();
            return fontList.size();
        }

    private:

        static std::unique_lock<std::mutex> getLock()
        {
            static std::mutex lockPtr;
            return std::unique_lock<std::mutex>(lockPtr);
        }

        typedef std::map<std::string, std::map<int, std::shared_ptr<KFontProperties>>> fontList_t;

        inline static fontList_t &getFontList()
        {
            static fontList_t fontList;
            return fontList;
        }

        static int getFirstGlyph(const TTF_Font *_Font)
        {
            int ch = 0;
            while(ch < 0x10000)
            {
                if(TTF_GlyphIsProvided(_Font, ch))
                {
                    return ch;
                }
                ch++;
            }
        }

        static int getLastGlyph(const TTF_Font *_Font)
        {
            int last = 0;
            int ch = 0;
            while(ch < 0x10000)
            {
                if(TTF_GlyphIsProvided(_Font, ch))
                {
                    last = ch;
                }
                ch++;
            }
            return last;
        }

        static int getTotalGlyphs(const TTF_Font *_Font)
        {
            int total = 0;
            int ch = 0;
            while(ch < 0x10000)
            {
                if(TTF_GlyphIsProvided(_Font, ch))
                {
                    total++;
                }
                ch++;
            }
            return total;
        }

        static std::vector<KFontProperties::GlyphRange> getRanges(const TTF_Font *_Font)
        {
            std::vector<KFontProperties::GlyphRange> ranges;
            // find glyph ranges.
            int ch = 0;
            while(ch < 0x10000)
            {
                if(TTF_GlyphIsProvided(_Font, ch++))
                {
                    int rs = ch - 1;
                    while(TTF_GlyphIsProvided(_Font, ch++))
                    {
                    }
                    ranges.push_back(KFontProperties::GlyphRange(rs, ch - 2));
                }
            }
            return ranges;
        }

        /**
         * Enumerate a specific file.
         * @param file The file to enumerate.
         * @param ptSize The point size of the font.
         * @return True if file was a ttf file.
         */
        static bool enumerateFont(const KayLib::KFile *file, int ptSize)
        {
            if(KString::strToLower(file->getExtension()) != ".ttf")
            {
                return false;
            }
            if(!file->isFile())
            {
                return false;
            }
            if(!TTF_WasInit())
            {
                TTF_Init();
            }
            auto lock = getLock();
            auto absPath = file->getAbsolutePath();
            fontList_t &fontList = getFontList();
            auto found = fontList.find(absPath);
            if(found != fontList.end())
            {
                auto ptList = found->second;
                if(ptList.find(ptSize) != ptList.end())
                {
                    // Already found.
                    return true;
                }
            }
            // New font found.
            TTF_Font *_Font = TTF_OpenFont(absPath.c_str(), ptSize);
            if(_Font != nullptr)
            {
                //std::cout << "found: " << absPath.c_str() << std::endl;
                // Create new object.
                KFontProperties * fp = new KFontProperties(_Font, file, ptSize);
                //std::shared_ptr<KFontProperties> fp = std::make_shared<KFontProperties>(_Font, file, ptSize);
                // Free the font.
                TTF_CloseFont(_Font);
                // Save the new properties.
                fontList[absPath][ptSize].reset(fp);
                return true;
            }
            return false;
        }

        /**
         * Enumerate fonts from the directory recursing to a depth of depth.
         * @param sPath the directory to enumerate.
         * @param depth the depth of the directory recursion.
         * @param ptSize The point size of the fonts to enumerate.
         */
        static void enumerateDirectory(const KFile &sPath, int depth, int ptSize)
        {
            if(sPath.isDirectory())
            {
                auto dContents = sPath.listFiles();
                for(auto file : dContents)
                {
                    if(file->isFile())
                    {
                        KFontProperties::enumerateFont(file.get(), ptSize);
                    }
                    if(file->isDirectory() && depth > 0)
                    {
                        enumerateDirectory(file->getName(), depth - 1, ptSize);
                    }
                }
            }
        }

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
        int glyphsInRange(const GlyphRange &range) const
        {
            int num = 0;

            for(auto rng : glyphRanges)
            {
                if(rng.start < range.start)
                {
                    if(rng.end >= range.start)
                    {
                        int min = (rng.end < range.end) ? rng.end : range.end;
                        num += (min - range.start) + 1;
                    }
                }
                else
                {
                    if(rng.start <= range.end)
                    {
                        int min = (rng.end < range.end) ? rng.end : range.end;
                        num += (min - rng.start) + 1;
                    }
                }
            }
            return num;
        }

    };

}

#endif /* KFONTPROPERTIES_H */

