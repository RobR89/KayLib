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

#ifndef KFONT_H
#define KFONT_H

#include "KFontProperties.h"
#include "../IO/KFile.h"

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace KayLib
{

    class KFont
    {
    private:

        KFont(const std::shared_ptr<KFontProperties> prop)
        {
            font = nullptr;
            fontColor = {0, 0, 0};
            backgroundColor = {255, 255, 255};
            quality = BLENDED;
        }

    public:

        virtual ~KFont()
        {
            if(font != nullptr)
            {
                TTF_CloseFont(font);
            }
        }

        enum TextQuality
        {
            DEFAULT, SOLID, SHADED, BLENDED
        };

        /**
         * Create a KFont object and load the specified font.
         * @param fontFile Name of the font. Or TTF font file.
         * @param ptSize The point size of the font to create.
         * @returns null if font not found.
         */
        static std::shared_ptr<KFont> create(const std::string &fontFile, int ptSize)
        {
            KFile fntFile = findFont(fontFile);
            if(!fntFile.isFile())
            {
                return std::shared_ptr<KFont>();
            }
            TTF_Font *font = TTF_OpenFont(fntFile.getAbsolutePath().c_str(), ptSize);
            if(font == nullptr)
            {
                return std::shared_ptr<KFont>();
            }
            std::shared_ptr<KFont> nFont(new KFont(KFontProperties::create(fntFile, ptSize)));
            auto &defaultFont = getDefaultFont();
            if(defaultFont.get() == nullptr)
            {
                defaultFont = nFont;
            }
            return nFont;
        }

        /**
         * Add a path to search for fonts if it exists.
         * @param path The path to add.
         */
        static bool addFontPath(const std::string &sPath)
        {
            KFile path(sPath);
            return addFontPath(path);
        }

        /**
         * Add a path to search for fonts if it exists.
         * @param path The path to add.
         */
        static bool addFontPath(const KFile &path)
        {
            // Is it a directory?
            if(path.isDirectory())
            {
                auto lock = getLock();
                std::vector<KFile> &fontPaths = getPaths();
                // yes, is the path already registered?
                if(std::find(fontPaths.begin(), fontPaths.end(), path) != fontPaths.end())
                {
                    return true;
                }
                // no, register it.
                fontPaths.push_back(path);
                return true;
            }
            return false;
        }

        /**
         * Get a copy list of font search paths.
         */
        static std::vector<KFile> getFontPaths()
        {
            auto lock = getLock();
            std::vector<KFile> &fontPaths = getPaths();
            return std::vector<KFile>(fontPaths);
        }


        /**
         * A default font to use. If null will be set to first successfully created font.
         */
        static std::shared_ptr<KFont> &getDefaultFont()
        {
            static std::shared_ptr<KFont> _defaultFont;
            return _defaultFont;
        }

        /**
         * Draw the text to a new SDL_Surface.
         * @param text The text to draw.
         * @param quality The quality to draw with.
         * @return The SDL surface.
         */
        SDL_Surface *drawText(const std::string &text, const TextQuality &quality = DEFAULT) const
        {
            return drawText(text, fontColor, backgroundColor, quality);
        }

        /**
         * Draw the text to a new SDL_Surface.
         * @param text The text to draw.
         * @param tColor The color of the text.
         * @param quality The quality to draw with.
         * @return The SDL surface.
         */
        SDL_Surface *drawText(const std::string &text, const SDL_Color &tColor, const TextQuality &quality = DEFAULT) const
        {
            return drawText(text, tColor, backgroundColor, quality);
        }

        /**
         * Draw the text to a new SDL_Surface.
         * @param text The text to draw.
         * @param tColor The color of the text.
         * @param bColor The background color.
         * @param quality The quality to draw with.
         * @return The SDL surface.
         */
        SDL_Surface *drawText(const std::string &text, const SDL_Color &tColor, const SDL_Color &bColor, const TextQuality &quality = DEFAULT) const
        {
            SDL_Surface *resulting_text;
            TextQuality useQuality = quality;
            if(useQuality == DEFAULT)
            {
                useQuality = quality;
            }

            if(useQuality == SOLID)
            {
                resulting_text = TTF_RenderUTF8_Solid(font, text.c_str(), tColor);
            }
            else if(useQuality == SHADED)
            {
                resulting_text = TTF_RenderUTF8_Shaded(font, text.c_str(), tColor, bColor);
            }
            else if(useQuality == BLENDED)
            {
                resulting_text = TTF_RenderUTF8_Blended(font, text.c_str(), tColor);
            }

            return resulting_text;
        }

        /**
         * Get the pixel length of the line of text.
         * @param text The line of text.
         * @return The pixel length of the text. -1 if the length cannot be determined.
         */
        int lineLenght(const std::string &text) const
        {
            if(font == nullptr)
            {
                return 0;
            }
            int w, h;
            int r = TTF_SizeText(font, text.c_str(), &w, &h);
            if(r != 0)
            {
                return -1;
            }
            return w;
        }

        /**
         * Get the number of pixels between lines.
         * @return The number of pixels between lines.
         */
        int LineHeight() const
        {
            if(fontProperties.get() == nullptr)
            {
                return 1;
            }
            return fontProperties->lineSkip;
        }

        /**
         * The font properties.
         */
        const std::shared_ptr<KFontProperties> fontProperties;

        /**
         * Color of the font if not specified in the function.
         */
        SDL_Color fontColor;

        /**
         * Background color for shaded render if not specified.
         */
        SDL_Color backgroundColor;

        /**
         * Quality of render if not specified.
         */
        TextQuality quality;

        /**
         * Get the font style.
         * @return The font style.
         */
        int getStyle() const
        {
            if(font == nullptr)
            {
                return 0;
            }
            return TTF_GetFontStyle(font);
        }

        /**
         * Set the font style.
         * @param style The file to set.
         */
        void setStyle(int style)
        {
            if(font == nullptr)
            {
                return;
            }
            TTF_SetFontStyle(font, style);
        }

        /**
         * Is this font a normal?
         * @return true if normal, false if styled.
         */
        bool isNormal() const
        {
            return (getStyle() & TTF_STYLE_NORMAL) == 0 ? false : true;
        }

        /**
         * Is this font bold?
         * @return true if bold.
         */
        bool isBold() const
        {
            return (getStyle() & TTF_STYLE_BOLD) == 0 ? false : true;
        }

        /**
         * Is this font italic?
         * @return true if italic.
         */
        bool isItalic() const
        {
            return (getStyle() & TTF_STYLE_ITALIC) == 0 ? false : true;
        }

        /**
         * Is this font underlined?
         * @return true if underlined.
         */
        bool isUnderline() const
        {
            return (getStyle() & TTF_STYLE_UNDERLINE) == 0 ? false : true;
        }

        /**
         * Is this font struck through?
         * @return true if struck through.
         */
        bool isStrikeThrough() const
        {
            return (getStyle() & TTF_STYLE_STRIKETHROUGH) == 0 ? false : true;
        }

        /**
         * Make this font normal.
         */
        void setNormal()
        {
            setStyle(0);
        }

        /**
         * Make this font bold.
         */
        void setBold()
        {
            setStyle(getStyle() | TTF_STYLE_BOLD);
        }

        /**
         * Make this font italic.
         */
        void setItalic()
        {
            setStyle(getStyle() | TTF_STYLE_ITALIC);
        }

        /**
         * Make this font underlined.
         */
        void setUnderline()
        {
            setStyle(getStyle() | TTF_STYLE_UNDERLINE);
        }

        /**
         * Make this font struck through.
         */
        void setStrikeThrough()
        {
            setStyle(getStyle() | TTF_STYLE_STRIKETHROUGH);
        }

        /**
         * Is this font monospaced.
         * @return True if monospaced.
         */
        bool isMonospaced() const
        {
            if(font == nullptr)
            {
                return false;
            }
            return TTF_FontFaceIsFixedWidth(font);
        }

        /**
         * Does this font have the glyph.
         * @param ch The glyph to check for.
         * @return True if this font has the glyph.
         */
        bool isGlyphProvided(int ch) const
        {
            if(font == nullptr)
            {
                return false;
            }
            return TTF_GlyphIsProvided(font, ch);
        }


    private:
        TTF_Font *font;

        static std::unique_lock<std::mutex> getLock()
        {
            static std::mutex lockPtr;
            return std::unique_lock<std::mutex>(lockPtr);
        }

        /**
         * A list of paths to search when trying to load fonts.
         */
        static std::vector<KFile> &getPaths()
        {
            static std::vector<KFile> fontPaths;
            return fontPaths;
        }
        //----------------------------------------------------------
        // font support functions

        static KFile findFont(const std::string font)
        {
            KFile fFile(font);
            // check for TTF file extension.
            std::string ext = fFile.getExtension();
            if(ext.length() > 0)
            {
                if(KString::strToLower(fFile.getExtension()) != ".ttf")
                {
                    return KFile();
                }
            }
            else
            {
                fFile = KFile(fFile.getName() + ".ttf");
            }
            // if the font name was specified with a full path then accept it.
            if(fFile.exists())
            {
                return fFile;
            }
            // Search through the font paths.
            auto fontPaths = getFontPaths();
            for(auto path : fontPaths)
            {
                // Is it a directory?
                if(path.isDirectory())
                {
                    // Yes, check for file.
                    KFile fPath(path.getPath() + "/" + fFile.getFilename());
                    if(fPath.exists())
                    {
                        // file exists!  Return the result.
                        return fPath;
                    }
                }
            }
            // font not found return empty path.
            return KFile();
        }

    };

}

#endif
