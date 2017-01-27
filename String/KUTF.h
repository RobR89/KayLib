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

/**
 * UTF library.  Just because I didn't like anything else available.
 * This library is endianness agnostic.  It sees a byte as a UTF8 character,
 * a word as a UTF16 character,  and a dword as a UTF32 character.  The order they
 * appear in memory or are loaded/saved to memory or files is irrelevant.
 */

#ifndef KUTF_H
#define KUTF_H

#include <string>
#include <stdio.h>

namespace KayLib
{

    class UTFCodeParser
    {
    public:
        UTFCodeParser();
        UTFCodeParser(const UTFCodeParser& orig);
        virtual ~UTFCodeParser();

        /**
         * Add a UTF8 character to the code.
         * @param c The UTF8 character.
         * @return number of additional characters needed. 0 = complete, -1 on Error,
         */
        int addChar(char c);
        /**
         * Add a UTF16 character to the code.
         * @param c The UTF16 character.
         * @return number of additional characters needed. 0 = complete, -1 on Error,
         */
        int addChar(char16_t c);
        /**
         * Add a UTF32 character to the code.
         * @param c The UTF32 character.
         * @return number of additional characters needed. 0 = complete, -1 on Error,
         */
        int addChar(char32_t c);

        /**
         * Get the current code as a UTF8 character string.
         * @return The UTF8 representation of the code of an empty string if the code is not valid.
         */
        std::string getUTF8();
        /**
         * Get the current code as a UTF16 character string.
         * @return The UTF16 representation of the code of an empty string if the code is not valid.
         */
        std::u16string getUTF16();
        /**
         * Get the current code as a UTF32 character string.
         * @return The UTF32 representation of the code of an empty string if the code is not valid.
         */
        std::u32string getUTF32();

        /**
         * Reset the state.
         */
        void reset();

        /**
         * Get the current character code value.
         * @return 
         */
        char32_t getCode()
        {
            return code;
        }

        /**
         * Is this current state a valid code?
         * @return True if code is valid.
         */
        bool valid()
        {
            return byte == 0;
        }

    private:
        char32_t code = 0;
        int byte = 0;

    };

    /**
     * Convert a 32 bit character code to a UTF8 string
     * @param code The UTF32 character.
     * @return The UTF8 string. Returns an empty string on failure.
     */
    std::string codeToUTF8(char32_t code);
    /**
     * Convert a 32 bit character code to a UTF16 string
     * @param code The UTF32 character.
     * @return The UTF16 string. Returns an empty string on failure.
     */
    std::u16string codeToUTF16(char32_t code);

    /**
     * Convert a UTF8 string to a UTF16 string
     * @param str The UTF8 string.
     * @return The UTF16 string. Returns an empty string on failure.
     */
    std::u16string utf8to16(std::string &str);
    /**
     * Convert a UTF8 string to a UTF32 string
     * @param str The UTF8 string.
     * @return The UTF32 string. Returns an empty string on failure.
     */
    std::u32string utf8to32(std::string &str);
    /**
     * Convert a UTF16 string to a UTF8 string
     * @param str The UTF16 string.
     * @return The UTF8 string. Returns an empty string on failure.
     */
    std::string utf16to8(std::u16string &str);
    /**
     * Convert a UTF16 string to a UTF32 string
     * @param str The UTF16 string.
     * @return The UTF32 string. Returns an empty string on failure.
     */
    std::u32string utf16to32(std::u16string &str);
    /**
     * Convert a UTF32 string to a UTF8 string
     * @param str The UTF16 string.
     * @return The UTF8 string. Returns an empty string on failure.
     */
    std::string utf32to8(std::u32string &str);
    /**
     * Convert a UTF32 string to a UTF16 string
     * @param str The UTF16 string.
     * @return The UTF16 string. Returns an empty string on failure.
     */
    std::u16string utf32to16(std::u32string &str);

    /**
     * Create the UTF escape sequence for the character "\\xCC".
     * @param c The character to create.
     * @return The escape sequence.
     */
    std::string utfEscapeChar(char c);
    /**
     * Create the UTF escape sequence for the character "\\uCCCC".
     * @param c The character to create.
     * @return The escape sequence.
     */
    std::string utfEscapeChar(char16_t c);
    /**
     * Create the UTF escape sequence for the character using the shortest form of "\\xCC" or "\\uCCCC" or "\\UCCCCCC".
     * @param c The character to create.
     * @return The escape sequence.
     */
    std::string utfEscapeChar(char32_t c);

    /**
     * Convert all unicode characters (>0x7F) in the string into escape sequences using utfEscapeChar().
     * @param str The string to convert.
     * @return The escaped string.
     */
    std::string utfEscape(std::string &str);
    /**
     * Convert all unicode escape sequences into actual character codes.
     * @param str The string to convert.
     * @return The un-escaped string.  Returns an empty string if there was an error.
     */
    std::string utfUnEscape(std::string &str);

}

#endif /* KUTF_H */

