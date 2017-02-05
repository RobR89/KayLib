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

#include "KString.h"

namespace KayLib
{

    class KUTF
    {
    public:

        class UTFCodeParser
        {
        public:

            UTFCodeParser() { }

            UTFCodeParser(const UTFCodeParser& orig)
            {
                code = orig.code;
                byte = orig.byte;
            }

            virtual ~UTFCodeParser() { }

            /**
             * Add a UTF8 character to the code.
             * @param c The UTF8 character.
             * @return number of additional characters needed. 0 = complete, -1 on Error,
             */
            int addChar(char c)
            {
                if(byte == 0)
                {
                    // First byte of sequence.
                    if((c & 0x80) == 0)
                    {
                        // Single byte code.
                        code = c;
                        return byte;
                    }
                    // Find number of bytes in code.
                    while(c & 0x80)
                    {
                        c <<= 1;
                        byte++;
                    }
                    code = c >> byte;
                    byte--;
                    if(byte == 0)
                    {
                        // Error: in middle of character.
                        code = 0;
                        return -1;
                    }
                    return byte;
                }
                // Continuing character.
                if((c & 0xC0) != 0x80)
                {
                    // Error: continuation code expected and not found.
                    code = 0;
                    return -1;
                }
                byte--;
                code <<= 6;
                code += c & 0x3f;
                return byte;
            }

            /**
             * Add a UTF16 character to the code.
             * @param c The UTF16 character.
             * @return number of additional characters needed. 0 = complete, -1 on Error,
             */
            int addChar(char16_t c)
            {
                if(byte == 0)
                {
                    if(c < 0xD800 || c >= 0xE000)
                    {
                        code = c;
                        return byte;
                    }
                    if(c >= 0xDC00)
                    {
                        // Error! This is a continuation character.
                        code = 0;
                        return -1;
                    }
                    byte = 1;
                    code = c & 0x3FF;
                    return byte;
                }
                if(c < 0xDC00 || c >= 0xE000)
                {
                    // Error: Expected continuation character not found.
                    if(c < 0xD800)
                    {
                        // Assume, good code following bad character.
                        code = c;
                        byte = 0;
                        return 0;
                    }
                    // Assume, good start code following bad character.
                    byte = 1;
                    code = c & 0x3FF;
                    return byte;
                }
                byte--;
                code <<= 10;
                code += c & 0x3FF;
                code += 0x010000;
                return byte;
            }

            /**
             * Add a UTF32 character to the code.
             * @param c The UTF32 character.
             * @return number of additional characters needed. 0 = complete, -1 on Error,
             */
            int addChar(char32_t c)
            {
                code = c;
                byte = 0;
                return byte;
            }

            /**
             * Get the current code as a UTF8 character string.
             * @return The UTF8 representation of the code of an empty string if the code is not valid.
             */
            std::string getUTF8()
            {
                if(byte == 0)
                {
                    return KUTF::codeToUTF8(code);
                }
                return "";
            }

            /**
             * Get the current code as a UTF16 character string.
             * @return The UTF16 representation of the code of an empty string if the code is not valid.
             */
            std::u16string getUTF16()
            {
                if(byte == 0)
                {
                    return KUTF::codeToUTF16(code);
                }
                return u"";
            }

            /**
             * Get the current code as a UTF32 character string.
             * @return The UTF32 representation of the code of an empty string if the code is not valid.
             */
            std::u32string getUTF32()
            {
                if(byte == 0)
                {
                    std::u32string out;
                    out += code;
                    return out;
                }
                return U"";
            }

            /**
             * Reset the state.
             */
            void reset()
            {
                code = 0;
                byte = 0;
            }

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
        static std::string codeToUTF8(char32_t code)
        {
            std::string out;
            if(code < 0x80)
            {
                out += (char) code;
                return out;
            }
            if(code < 0x800)
            {
                out += (char) (0xC0 + ((code >> 6) & 0x1f));
                out += (char) (0x80 + (code & 0x3f));
                return out;
            }
            if(code < 0x10000)
            {
                out += (char) (0xE0 + ((code >> 12) & 0x1f));
                out += (char) (0x80 + ((code >> 6) & 0x3f));
                out += (char) (0x80 + (code & 0x3f));
                return out;
            }
            if(code < 0x200000)
            {
                out += (char) (0xF0 + ((code >> 18) & 0x1f));
                out += (char) (0x80 + ((code >> 12) & 0x3f));
                out += (char) (0x80 + ((code >> 6) & 0x3f));
                out += (char) (0x80 + (code & 0x3f));
                return out;
            }
            if(code < 0x4000000)
            {
                out += (char) (0xF8 + ((code >> 24) & 0x1f));
                out += (char) (0x80 + ((code >> 18) & 0x3f));
                out += (char) (0x80 + ((code >> 12) & 0x3f));
                out += (char) (0x80 + ((code >> 6) & 0x3f));
                out += (char) (0x80 + (code & 0x3f));
                return out;
            }
            out += (char) (0xFC + ((code >> 30) & 0x1f));
            out += (char) (0x80 + ((code >> 24) & 0x3f));
            out += (char) (0x80 + ((code >> 18) & 0x3f));
            out += (char) (0x80 + ((code >> 12) & 0x3f));
            out += (char) (0x80 + ((code >> 6) & 0x3f));
            out += (char) (0x80 + (code & 0x3f));
            return out;
        }

        /**
         * Convert a 32 bit character code to a UTF16 string
         * @param code The UTF32 character.
         * @return The UTF16 string. Returns an empty string on failure.
         */
        static std::u16string codeToUTF16(char32_t code)
        {
            std::u16string out;
            if(code < 0x010000)
            {
                out += (char16_t)code;
                return out;
            }
            char32_t ch = code - 0x010000;
            char32_t cl = ch & 0x03FF;
            ch >>= 10;
            out += (0xD800 + ch);
            out += (0xDC00 + cl);
            return out;
        }

        /**
         * Convert a UTF8 string to a UTF16 string
         * @param str The UTF8 string.
         * @return The UTF16 string. Returns an empty string on failure.
         */
        static std::u16string utf8to16(std::string &str)
        {
            std::u16string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return u"";
                }
                if(res == 0)
                {
                    out += code.getUTF16();
                }
            }
            return out;
        }

        /**
         * Convert a UTF8 string to a UTF32 string
         * @param str The UTF8 string.
         * @return The UTF32 string. Returns an empty string on failure.
         */
        static std::u32string utf8to32(std::string &str)
        {
            std::u32string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return U"";
                }
                if(res == 0)
                {
                    out += code.getUTF32();
                }
            }
            return out;
        }

        /**
         * Convert a UTF16 string to a UTF8 string
         * @param str The UTF16 string.
         * @return The UTF8 string. Returns an empty string on failure.
         */
        static std::string utf16to8(std::u16string &str)
        {
            std::string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return "";
                }
                if(res == 0)
                {
                    out += code.getUTF8();
                }
            }
            return out;
        }

        /**
         * Convert a UTF16 string to a UTF32 string
         * @param str The UTF16 string.
         * @return The UTF32 string. Returns an empty string on failure.
         */
        static std::u32string utf16to32(std::u16string &str)
        {
            std::u32string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return U"";
                }
                if(res == 0)
                {
                    out += code.getUTF32();
                }
            }
            return out;
        }

        /**
         * Convert a UTF32 string to a UTF8 string
         * @param str The UTF16 string.
         * @return The UTF8 string. Returns an empty string on failure.
         */
        static std::string utf32to8(std::u32string &str)
        {
            std::string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return "";
                }
                if(res == 0)
                {
                    out += code.getUTF8();
                }
            }
            return out;
        }

        /**
         * Convert a UTF32 string to a UTF16 string
         * @param str The UTF16 string.
         * @return The UTF16 string. Returns an empty string on failure.
         */
        static std::u16string utf32to16(std::u32string &str)
        {
            std::u16string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return u"";
                }
                if(res == 0)
                {
                    out += code.getUTF16();
                }
            }
            return out;
        }

        /**
         * Create the UTF escape sequence for the character "\\xCC".
         * @param c The character to create.
         * @return The escape sequence.
         */
        static std::string utfEscapeChar(char c)
        {
            return "\\x" + KString::toHex(c);
        }

        /**
         * Create the UTF escape sequence for the character "\\uCCCC".
         * @param c The character to create.
         * @return The escape sequence.
         */
        static std::string utfEscapeChar(char16_t c)
        {
            char low = c & 0x0ff;
            char high = (c >> 8) & 0x0ff;
            return "\\u" + KString::toHex(high) + KString::toHex(low);
        }

        /**
         * Create the UTF escape sequence for the character using the shortest form of "\\xCC" or "\\uCCCC" or "\\UCCCCCC".
         * @param c The character to create.
         * @return The escape sequence.
         */
        static std::string utfEscapeChar(char32_t c)
        {
            char low1 = c & 0x0ff;
            char high1 = (c >> 8) & 0x0ff;
            char low2 = (c >> 16) & 0x0ff;
            char high2 = (c >> 24) & 0x0ff;
            if(high2 == 0 && low2 == 0)
            {
                if(high1 == 0)
                {
                    return "\\x" + KString::toHex(low1);
                }
                return "\\u" + KString::toHex(high1) + KString::toHex(low1);
            }
            return "\\U" + KString::toHex(high2) + KString::toHex(low2) + KString::toHex(high1) + KString::toHex(low1);
        }

        /**
         * Convert all unicode characters (>0x7F) in the string into escape sequences using utfEscapeChar().
         * @param str The string to convert.
         * @return The escaped string.
         */
        static std::string utfEscape(std::string &str)
        {
            std::string out;
            UTFCodeParser code;
            for(auto c : str)
            {
                int res = code.addChar(c);
                if(res == -1)
                {
                    // There was an error.
                    return "";
                }
                if(res == 0)
                {
                    if(code.getCode() < 0x80)
                    {
                        out += code.getUTF8();
                    }
                    else
                    {
                        out += utfEscapeChar(code.getCode());
                    }
                }
            }
            return out;
        }

        /**
         * Convert all unicode escape sequences into actual character codes.
         * @param str The string to convert.
         * @return The un-escaped string.  Returns an empty string if there was an error.
         */
        static std::string utfUnEscape(std::string &str)
        {
            std::string out;
            bool esc = false;
            int sz = -1;
            char32_t code;
            for(auto c : str)
            {
                if(esc)
                {
                    // We are in an escape.
                    if(sz == -1)
                    {
                        // This is the escape character.
                        if(c != 'u' && c != 'U' && c != 'x')
                        {
                            // Was not a unicode escape.
                            esc = false;
                            out += "\\" + c;
                            continue;
                        }
                        // This is a unicode escape start reading it.
                        sz = 4; // UTF16
                        if(c == 'U')
                        {
                            sz = 8; // UTF32
                        }
                        if(c == 'x')
                        {
                            sz = 2; // UTF8
                        }
                        code = 0;
                        continue;
                    }
                    // We are in a unicode escape sequence.
                    int val = KString::digitHex(c);
                    if(val == -1)
                    {
                        // Error parsing unicode sequence.
                        return "";
                    }
                    code <<= 4;
                    code += val;
                    sz--;
                    if(sz > 0)
                    {
                        continue;
                    }
                    // End of sequence.
                    out += KUTF::codeToUTF8(code);
                    esc = false;
                    continue;
                }
                if(c == '\\')
                {
                    // This is an escape sequence.
                    esc = true;
                    sz = -1;
                    continue;
                }
                // Just part of the string, pass it through.
                out += c;
            }
            if(esc && sz > 0)
            {
                // Technically this is an error...
                // Add the code if we ended the string while parsing a unicode escape.
                out += KUTF::codeToUTF8(code);
            }
            return out;
        }
    };

}

#endif /* KUTF_H */

