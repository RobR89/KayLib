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
        KFont(const std::shared_ptr<KFontProperties> prop);

    public:
        virtual ~KFont();

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
        static std::shared_ptr<KFont> create(const std::string &fontFile, int ptSize);

        /**
         * Add a path to search for fonts if it exists.
         * @param path The path to add.
         */
        static bool addFontPath(const std::string &path);
        /**
         * Add a path to search for fonts if it exists.
         * @param path The path to add.
         */
        static bool addFontPath(const KFile &path);

        /**
         * Get a list of font search paths.
         */
        static std::vector<KFile> getFontPaths();

        /**
         * A default font to use. If null will be set to first successfully created font.
         */
        static std::shared_ptr<KFont> defaultFont;

        /**
         * Draw the text to a new SDL_Surface.
         * @param text The text to draw.
         * @param quality The quality to draw with.
         * @return The SDL surface.
         */
        SDL_Surface *drawText(const std::string &text, const TextQuality &quality = DEFAULT) const;
        /**
         * Draw the text to a new SDL_Surface.
         * @param text The text to draw.
         * @param tColor The color of the text.
         * @param quality The quality to draw with.
         * @return The SDL surface.
         */
        SDL_Surface *drawText(const std::string &text, const SDL_Color &tColor, const TextQuality &quality = DEFAULT) const;
        /**
         * Draw the text to a new SDL_Surface.
         * @param text The text to draw.
         * @param tColor The color of the text.
         * @param bColor The background color.
         * @param quality The quality to draw with.
         * @return The SDL surface.
         */
        SDL_Surface *drawText(const std::string &text, const SDL_Color &tColor, const SDL_Color &bColor, const TextQuality &quality = DEFAULT) const;

        /**
         * Get the pixel length of the line of text.
         * @param text The line of text.
         * @return The pixel length of the text. -1 if the length cannot be determined.
         */
        int lineLenght(const std::string &text) const;

        /**
         * Get the number of pixels between lines.
         * @return The number of pixels between lines.
         */
        int LineHeight() const;

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
        int getStyle() const;
        /**
         * Set the font style.
         * @param style The file to set.
         */
        void setStyle(int style);
        /**
         * Is this font a normal?
         * @return true if normal, false if styled.
         */
        bool isNormal() const;
        /**
         * Is this font bold?
         * @return true if bold.
         */
        bool isBold() const;
        /**
         * Is this font italic?
         * @return true if italic.
         */
        bool isItalic() const;
        /**
         * Is this font underlined?
         * @return true if underlined.
         */
        bool isUnderline() const;
        /**
         * Is this font struck through?
         * @return true if struck through.
         */
        bool isStrikeThrough() const;
        /**
         * Make this font normal.
         */
        void setNormal();
        /**
         * Make this font bold.
         */
        void setBold();
        /**
         * Make this font italic.
         */
        void setItalic();
        /**
         * Make this font underlined.
         */
        void setUnderline();
        /**
         * Make this font struck through.
         */
        void setStrikeThrough();

        /**
         * Is this font monospaced.
         * @return True if monospaced.
         */
        bool isMonospaced() const;
        /**
         * Does this font have the glyph.
         * @param ch The glyph to check for.
         * @return True if this font has the glyph.
         */
        bool isGlyphProvided(int ch) const;

    private:
        TTF_Font *font;

    };

}

#endif
