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

#ifndef KSTRING_H
#define KSTRING_H

#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>

namespace KayLib
{

    class KString
    {
    public:

        /**
         * Convert a character to its numeric digit value.
         * @param c The character to convert.
         * @return The value (0-9) or -1 if it was not a digit.
         */
        static int digit(const int c)
        {
            int d = c - '0';
            if(d >= 0 && d <= 9)
            {
                return d;
            }
            return -1;
        }

        /**
         * Convert a character to its numeric digit value.
         * @param c The character to convert.
         * @return The value (0-15) or -1 if it was not a digit.
         */
        static int digitHex(const int c)
        {
            int d = c - '0';
            if(d >= 0 && d <= 9)
            {
                return d;
            }
            d = c - 'a';
            if(d >= 0 && d <= 5)
            {
                return d + 10;
            }
            d = c - 'A';
            if(d >= 0 && d <= 5)
            {
                return d + 10;
            }
            return -1;
        }

        /**
         * Create a string containing the hex values of the data.
         * @param data The data to convert.
         * @param length The length of the data.
         * @param separator A string to insert between each 2 byte hex code.
         * @return The created string.
         */
        static std::string toHex(const unsigned char *data, const int length, const std::string separator)
        {
            char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
            std::string hex;
            for(int i = 0; i < length; i++)
            {
                unsigned char v = data[i];
                unsigned char low = v & 0x0f;
                unsigned char high = (v >> 4) & 0x0f;
                hex += hexChar[high];
                hex += hexChar[low];
                if(i < length - 1)
                {
                    hex += separator;
                }
            }
            return hex;
        }

        /**
         * Create a string containing the hex values of the data.
         * @param data The data to convert.
         * @param length The length of the data.
         * @return The created string.
         */
        static std::string toHex(const unsigned char *data, const int length)
        {
            char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
            std::string hex;
            for(int i = 0; i < length; i++)
            {
                unsigned char v = data[i];
                unsigned char low = v & 0x0f;
                unsigned char high = (v >> 4) & 0x0f;
                hex += hexChar[high];
                hex += hexChar[low];
            }
            return hex;
        }

        /**
         * Create a string containing the 2 character hex value.
         * @param value The value to convert.
         * @return The created string.
         */
        static std::string toHex(const unsigned char value)
        {
            char hexChar[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
            unsigned char low = value & 0x0f;
            unsigned char high = (value >> 4) & 0x0f;
            std::string hex;
            hex += hexChar[high];
            hex += hexChar[low];
            return hex;
        }

        /**
         * Convert the string to all lower case.
         * @param str The string to convert.
         * @return The converted string.
         */
        static std::string strToLower(std::string str)
        {
            std::string str2;
            str2.resize(str.size());

            std::transform(str.begin(), str.end(), str2.begin(), std::ptr_fun<int, int>(std::tolower));

            return str2;
        }

        /**
         * Convert the string to all upper case.
         * @param str The string to convert.
         * @return The converted string.
         */
        static std::string strToUpper(std::string str)
        {
            std::string str2;
            str2.resize(str.size());

            std::transform(str.begin(), str.end(), str2.begin(), std::ptr_fun<int, int>(std::toupper));

            return str2;
        }

        /**
         * Does the string 'str' begin with the string 'begin'?
         * @param str The string to check.
         * @param begin The string to look for
         * @return true if 'str' begins with 'begin'
         */
        static bool beginsWith(const std::string &str, const std::string &begin)
        {
            return str.find(begin) == 0;
        }

        /**
         * Does the string 'str' end with the string 'end'?
         * @param str The string to check.
         * @param end The string to look for
         * @return true if 'str' ends with 'end'
         */
        static bool endsWith(const std::string &str, const std::string &end)
        {
            if(&str != nullptr && &end != nullptr)
            {
                int sLen = str.length();
                int eLen = end.length();
                if(sLen >= eLen)
                {
                    if(str.substr(sLen - eLen).compare(end) == 0)
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        /**
         * Converts non-escaped characters to escape characters.
         * @param str The string to convert.
         * @param assumeEscapes Assume that backslashes may already be escaping characters.
         * @return The converted string.
         * @note If assumeEscapes is true the function will preserve current escape sequences
         * for backslash and quote "\\"" will remain as "\\"" and "\\\\" as "\\\\".
         * If it is false the backslash and quotes will be escaped "\\"" will become "\\\\\\""
         * and "\\\\" will become "\\\\\\\\".
         */
        static std::string escape(const std::string &str, bool assumeEscapes = false)
        {
            std::stringstream out;
            int len = str.length();
            for(int i = 0; i < len; i++)
            {
                char c = str[i];
                switch(c)
                {
                    case '\a': // 0x07
                        out << "\\a";
                        continue;
                    case '\b': // 0x08
                        out << "\\b";
                        continue;
                    case '\t': // 0x09
                        out << "\\t";
                        continue;
                    case '\n': // 0x0a
                        out << "\\n";
                        continue;
                    case '\v': // 0x0b
                        out << "\\v";
                        continue;
                    case '\f': // 0x0c
                        out << "\\f";
                        continue;
                    case '\r': // 0x0d
                        out << "\\r";
                        continue;
                    case '\"':
                        out << "\\\"";
                        continue;
                    case '\'':
                        out << "\\\'";
                        continue;
                    case '\\':
                        if(i < len - 1 && assumeEscapes)
                        {
                            // backslash or quote already escaped.
                            if(str[i + 1] == '\\' || str[i + 1] == '\"' || str[i + 1] == '\'')
                            {
                                i++;
                                out << c << str[i];
                                continue;
                            }
                        }
                        out << "\\\\";
                        continue;
                    default:
                        if(c <= 0x0F)
                        {
                            out << "\\u00" + toHex(c);
                        }
                        else
                        {
                            out << c;
                        }
                        continue;
                }
            }
            return out.str();
        }

        /**
         * Converts escaped characters to non-escape characters.
         * @param str The string to convert.
         * @return The converted string.
         */
        static std::string unescape(const std::string &str)
        {
            std::stringstream out;
            int len = str.length();
            for(int i = 0; i < len; i++)
            {
                char c = str[i];
                if(c != '\\')
                {
                    // Not an escape sequence.
                    out << c;
                    continue;
                }
                // An escape sequence.
                i++;
                if(i >= len)
                {
                    // Error: occured at end of line.
                    // Output and end.
                    out << c;
                    break;
                }
                c = str[i];
                switch(c)
                {
                    case 'a': // 0x07
                        out << "\a";
                        continue;
                    case 'b': // 0x08
                        out << "\b";
                        continue;
                    case 't': // 0x09
                        out << "\t";
                        continue;
                    case 'n': // 0x0a
                        out << "\n";
                        continue;
                    case 'v': // 0x0b
                        out << "\v";
                        continue;
                    case 'f': // 0x0c
                        out << "\f";
                        continue;
                    case 'r': // 0x0d
                        out << "\r";
                        continue;
                    case '\"':
                        out << "\"";
                        continue;
                    case '\'':
                        out << "\'";
                        continue;
                    case '\\':
                        out << "\\";
                        continue;
                    default:
                        // Unknown escape sequence, output as-is and continue.
                        if(c <= 0x0F)
                        {
                            out << "\\\\u00" + toHex(c);
                        }
                        else
                        {
                            out << "\\" << c;
                        }
                        continue;
                }
            }
            return out.str();
        }

        /**
         * Convert the string for use in an xml document.
         * @param str The string to convert.
         * @return The converted string.
         */
        static std::string xmlEscape(const std::string &str)
        {
            std::stringstream out;
            int len = str.length();
            for(int i = 0; i < len; i++)
            {
                char c = str[i];
                switch(c)
                {
                    case '<':
                        out << "&lt;";
                        continue;
                    case '>':
                        out << "&gt;";
                        continue;
                    case '&':
                        out << "&amp;";
                        continue;
                    case '\"':
                        out << "&quot;";
                        continue;
                    case '\'':
                        out << "&apos;";
                        continue;
                    default:
                        out << c;
                }
            }
            return out.str();
        }

        /**
         * Convert the xml string to a normal string.
         * @param str The string to convert.
         * @return The converted string.
         */
        static std::string xmlUnescape(const std::string &str)
        {
            std::stringstream out;
            int len = str.length();
            for(int i = 0; i < len; i++)
            {
                char c = str[i];
                if(c != '&')
                {
                    // Not an escape sequence.
                    out << c;
                    continue;
                }
                int j = i;
                while(j < len && c != ';' && j - i <= 6)
                {
                    c = str[++j];
                }
                if(c != ';')
                {
                    // Unknown escape sequence, output & and continue.
                    out << "&";
                    continue;
                }
                std::string token = str.substr(i, (j - i) + 1);
                token = strToLower(token);
                if(token == "&lt;")
                {
                    i = j;
                    out << "<";
                    continue;
                }
                if(token == "&gt;")
                {
                    i = j;
                    out << ">";
                    continue;
                }
                if(token == "&quot;")
                {
                    i = j;
                    out << "\"";
                    continue;
                }
                if(token == "&amp;")
                {
                    i = j;
                    out << "&";
                    continue;
                }
                if(token == "&apos;")
                {
                    i = j;
                    out << "\'";
                    continue;
                }
                // Unknown escape sequence, output & and continue.
                out << "&";
            }
            return out.str();
        }

    };
}

#endif /* KSTRING_H */

