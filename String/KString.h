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

#ifndef KSTRING_H
#define	KSTRING_H

#include <string>
#include <stdio.h>

namespace KayLib {

/**
 * Convert a character to its numeric digit value.
 * @param c The character to convert.
 * @return The value (0-9) or -1 if it was not a digit.
 */
int digit(const int c);

/**
 * Convert a character to its numeric digit value.
 * @param c The character to convert.
 * @return The value (0-15) or -1 if it was not a digit.
 */
int digitHex(const int c);

/**
 * Create a string containing the hex values of the data.
 * @param data The data to convert.
 * @param length The length of the data.
 * @param separator A string to insert between each 2 byte hex code.
 * @return The created string.
 */
std::string toHex(const unsigned char *data, const int length, const std::string separator);
/**
 * Create a string containing the hex values of the data.
 * @param data The data to convert.
 * @param length The length of the data.
 * @return The created string.
 */
std::string toHex(const unsigned char *data, const int length);
/**
 * Create a string containing the 2 character hex value.
 * @param value The value to convert.
 * @return The created string.
 */
std::string toHex(const unsigned char value);
/**
 * Does the string 'str' begin with the string 'begin'?
 * @param str The string to check.
 * @param begin The string to look for
 * @return true if 'str' begins with 'begin'
 */
bool beginsWith(const std::string &str, const std::string &begin);
/**
 * Does the string 'str' end with the string 'end'?
 * @param str The string to check.
 * @param end The string to look for
 * @return true if 'str' ends with 'end'
 */
bool endsWith(const std::string &str, const std::string &end);

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
std::string escape(const std::string &str, bool assumeEscapes = false);

/**
 * Convert the string for use in an xml document.
 * @param str The string to convert.
 * @return The converted string.
 */
std::string xmlEscape(const std::string &str);

}

#endif	/* KSTRING_H */

