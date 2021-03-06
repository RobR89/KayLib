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

#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include <string>
#include <cmath>
#include "../String/KString.h"
#include "../String/KUTF.h"

namespace KayLib
{

    template<typename T>
    class StringParser
    {
    public:

        StringParser(const std::basic_string<T> &str) : string(str)
        {
            index = 0;
            length = string.length();
            tabAsWhitespace = true;
        }

        /**
         * Get the current index being parsed.
         * @return The index.
         */
        int getIndex() const
        {
            return index;
        }

        /**
         * Set the current index being parsed.
         * @param i The index to set.
         * @return The index that was actually set.
         */
        int setIndex(int i) const
        {
            index = i;
            if(index < 0)
            {
                index = 0;
            }
            if(index >= length)
            {
                index = length;
            }
            return index;
        }

        /**
         * Set how tab characters '\t' should be considered in whitespace checks.
         * @param tab True if tabs are whitespace.
         */
        void setTabAsWhitespace(bool tab)
        {
            tabAsWhitespace = tab;
        }

        /**
         * Moves the index back one character.
         */
        void back()
        {
            if(index > 0)
            {
                index--;
            }
        }

        /**
         * Skip the next 'num' characters.
         * @param num The number of characters to skip.
         */
        void skip(int num)
        {
            index += num;
            if(index > length)
            {
                index = length;
            }
        }

        /**
         * Get the string being parsed.
         * @return A copy of the string being parsed.
         */
        std::basic_string<T> getParseString() const
        {
            return std::basic_string<T>(string);
        }

        /**
         * Check if the parser has reached the end of the string.
         * @return True if the parser has reached the end of the string.
         */
        bool isEnd() const
        {
            return index >= length;
        }

        /**
         * Is the next character part of a newline designator '\r' or '\n'?
         * @return True if the next char is part of a newline.
         */
        bool isNewline() const
        {
            T c = peekChar();
            return (c == '\r' || c == '\n');
        }

        /**
         * Is the next character a numerical digit?
         * @return True if the next character is a digit.
         */
        bool isDigit() const
        {
            return KString::digit(peekChar()) != -1;
        }

        /**
         * Checks to see if the next number value is an integer or decimal.
         * @return True if the number has a decimal.
         */
        bool isDecimal()
        {
            int i = index;
            if(string[i] == '-' || string[i] == '+')
            {
                i++;
            }
            int v;
            while((v = KString::digit(string[i])) != -1 && i < length)
            {
                i++;
            }
            return string[i] == '.';
        }

        /**
         * Reset the parser to the beginning of the string.
         */
        void reset()
        {
            index = 0;
        }

        /**
         * Gets the next character but does not advance the pointer.
         * @return The next character in the string.
         */
        T peekChar() const
        {
            if(index >= length)
            {
                return 0;
            }
            return string[index];
        }

        /**
         * Gets the next character and advances the pointer.
         * @return The next character.
         */
        T getChar()
        {
            if(index >= length)
            {
                return 0;
            }
            return string[index++];
        }

        /**
         * Gets next word (space or tab separated) from the string.
         * @return The next word.
         */
        std::basic_string<T> getWord()
        {
            int start = index;
            int sz = 0;
            T c;
            while((c = peekChar()) != ' ' && index < length)
            {
                if(c == '\t')
                {
                    break;
                }
                sz++;
                index++;
            }
            if(sz == 0)
            {
                return "";
            }
            return string.substr(start, sz);
        }

        /**
         * Gets a string from the current position to the separator or end of string or optionally newline if the separator is not found.
         * @param separator The word separator to look for.
         * @param includeNewline True if newlines should be included in the fetched string.
         * @return The string.
         */
        std::basic_string<T> getTo(const T separator, bool includeNewline = true)
        {
            int start = index;
            int sz = 0;
            while(peekChar() != separator && index < length && (includeNewline || !isNewline()))
            {
                sz++;
                index++;
            }
            return string.substr(start, sz);
        }

        /**
         * Gets an integer value from the string.
         * @return The integer value.
         * @note Recognizes sign characters.
         */
        int getInt()
        {
            bool neg = false;
            if(peekChar() == '-')
            {
                neg = true;
                index++;
            }
            if(peekChar() == '+' && !neg)
            {
                index++;
            }
            int value = 0;
            int v;
            while((v = KString::digit(peekChar())) != -1 && index < length)
            {
                value *= 10;
                value += v;
                index++;
            }
            return neg ? -value : value;
        }

        /**
         * Gets a long value from the string.
         * @return The long value.
         * @note Recognizes sign characters.
         */
        long getLong()
        {
            bool neg = false;
            if(peekChar() == '-')
            {
                neg = true;
                index++;
            }
            if(peekChar() == '+' && !neg)
            {
                index++;
            }
            long value = 0;
            int v;
            while((v = KString::digit(peekChar())) != -1 && index < length)
            {
                value *= 10;
                value += v;
                index++;
            }
            return neg ? -value : value;
        }

        /**
         * Gets a hexadecimal value from the string.
         * @return The hexadecimal value.
         */
        long getHex()
        {
            long value = 0;
            int v;
            while((v = digitHex(peekChar())) != -1 && index < length)
            {
                value *= 16;
                value += v;
                index++;
            }
            return value;
        }

        /**
         * Gets a double value in from the string.
         * @return The double value.
         * @note Recognizes sign characters.
         * @note Recognizes exponential notation.
         */
        double getDouble()
        {
            bool neg = false;
            if(peekChar() == '-')
            {
                neg = true;
                index++;
            }
            if(peekChar() == '+' && !neg)
            {
                index++;
            }
            double value = 0;
            int v;
            while((v = KString::digit(peekChar())) != -1 && index < length)
            {
                value *= 10;
                value += v;
                index++;
            }
            // Check for decimal.
            if(peekChar() == '.')
            {
                index++;
                double div = 1;
                while((v = KString::digit(peekChar())) != -1 && index < length)
                {
                    div *= 10;
                    value *= 10;
                    value += v;
                    index++;
                }
                value /= div;
            }
            // Check for exponent.
            if(peekChar() == 'e' || peekChar() == 'E')
            {
                index++;
                // Exponential value.
                double exp = getDouble();
                // Raise 10 to the power of the exponent.
                exp = std::pow(10.0, exp);
                // Multiply by the exponent.
                value = value * exp;
            }
            return neg ? -value : value;
        }

        /**
         * Gets the date if in the form yyyy-MM-ddXhh:mm:ss where X is ignored.
         * @return The date as a linux timestamp.
         */
        long getDate()
        {
            struct tm t;
            t.tm_year = getInt() - 1900;
            if(peekChar() != '-')
            {
                return NULL;
            }
            index++;
            t.tm_mon = getInt() - 1;
            if(peekChar() != '-')
            {
                return NULL;
            }
            index++;
            t.tm_mday = getInt();
            // get the any char.
            getChar();
            t.tm_hour = getInt() - 1;
            if(peekChar() != ':')
            {
                return NULL;
            }
            index++;
            t.tm_min = getInt() - 1;
            if(peekChar() != ':')
            {
                return NULL;
            }
            index++;
            t.tm_sec = getInt() - 1;
            // Get the time in seconds.
            long tt = mktime(&t);
            // Convert to milliseconds.
            tt *= 1000;
            if(peekChar() != '.')
            {
                // fraction of seconds.
                double fract = getDouble();
                tt += (long) fract * 1000;
            }
            return tt;
        }

        /**
         * Check if the next character is a whitespace character.
         * @param andNewLine True if newlines should be counted as whitespace as well.
         */
        bool isWhitespace(bool andNewLine)
        {
            T c = peekChar();
            return (c == ' ' || (c == '\t' && tabAsWhitespace) || ((c == '\r' || c == '\n') && andNewLine));
        }

        /**
         * Skip all whitespace from current index.
         * @param andNewLine True if newlines should also be skipped.
         */
        void skipWhitespace(bool andNewLine)
        {
            while(isWhitespace(andNewLine) && index < length)
            {
                index++;
            }
        }

        /**
         * Gets the string contained in single or double quotes.
         * @return The string.
         * @note The index must be pointing to the initial quote.
         * @note If the string is contained in single quotes double quotes will be ignored.
         * @note If the string is contained in double quotes single quotes will be ignored.
         */
        std::basic_string<T> getQuotedString()
        {
            T quote = peekChar();
            if(quote != '\"' && quote != '\'')
            {
                return "";
            }
            index++;
            int start = index;
            int sz = 0;
            T c;
            while((c = getChar()) != quote && index < length)
            {
                sz++;
                // Check for escape character.
                if(c == '\\')
                {
                    // We have an escape character, skip it.
                    sz++;
                    index++;
                }
            }
            if(sz == 0)
            {
                return "";
            }
            return string.substr(start, sz);
        }

        /**
         * Get all characters till the next newline
         * @return The returned string.
         */
        std::basic_string<T> getToEOL()
        {
            int start = index;
            int sz = 0;
            while(!isNewline())
            {
                index++;
                sz++;
            }
            if(sz == 0)
            {
                return "";
            }
            return string.substr(start, sz);
        }

        /**
         * Get the 'size' characters starting at 'start'.
         * @param start The first character.
         * @param size The number of characters.
         * @return The string.
         */
        std::basic_string<T> getRange(int start, int size)
        {
            if(start < 0 || start >= length || size < 1 || start + size > length)
            {
                return "";
            }
            return string.substr(start, size);
        }

        /**
         * Check if the next part of the string matches the query.
         * @param next The string to check for.
         * @param advance Should the index be advanced if the string matches?
         * @return True if the string is found.
         */
        bool nextIs(const std::basic_string<T> &next, bool advance = false)
        {
            if(next.empty())
            {
                return true;
            }
            int len = next.length();
            for(int i = 0; i < len; i++)
            {
                if(string[index + i] != next[i])
                {
                    return false;
                }
            }
            if(advance)
            {
                index += len;
            }
            return true;
        }

        /**
         * Peek at the next UTF encoded character.
         * @return The UTF character or -1 on failure.
         */
        char32_t peekCharUTF()
        {
            if(index >= length)
            {
                return -1;
            }
            int i = index;
            int res;
            while((res = code.addChar(string[i])) != 0 && i < length)
            {
                i++;
            }
            if(res != 0)
            {
                // There was an error.
                code.reset();
                return -1;
            }
            return code.getCode();
        }

        /**
         * Get the next UTF encoded character.
         * @return The UTF character or -1 on failure.
         */
        char32_t getCharUTF()
        {
            if(index >= length)
            {
                return -1;
            }
            int res;
            do
            {
                res = code.addChar(string[index++]);
            }
            while(res != 0 && index < length);
            if(res != 0)
            {
                // There was an error.
                code.reset();
                return -1;
            }
            return code.getCode();
        }

        /**
         * Gets next word (space or tab separated) from the string.
         * @return The next word.
         */
        std::basic_string<T> getWordUTF()
        {
            std::basic_string<T> out;
            char32_t c;
            char32_t n = -1;
            while((c = getCharUTF()) != ' ')
            {
                if(c == n || c == '\t')
                {
                    return out;
                }
                // 'code' still has our character so convert it to UTF8.
                out += code.getUTF<T>();
            }
            return out;
        }

    protected:
        KUTF::UTFCodeParser code;
        const std::basic_string<T> string;
        int index;
        int length;
        bool tabAsWhitespace;

    };

}

#endif /* STRINGPARSER_H */

