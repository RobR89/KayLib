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

#ifndef STRINGTEST_H
#define STRINGTEST_H

#include <iostream>
#include <vector>

//-------------------------------------------------------------------------
// String parser tests

#include "Parser/StringParser.h"

using namespace KayLib;

bool testStringParser()
{
    std::vector<int> chrCodes = {122, 27700, 119051, 32, 72, 101, 108, 108, 111, 33};
    std::vector<std::string> words = {"z\346\260\264\360\235\204\213", "Hello!"};
    std::string utf8 = u8"z\u6c34\U0001d10b Hello!";
    std::u16string wutf = u"z\u6c34\U0001d10b Hello!";

    std::cout << "Starting string parser tests." << std::endl;

    std::cout << "Using UTF8 parser with string: " << utf8 << std::endl;
    StringParserUTF8 p(utf8);
    std::cout << "Character codes: ";
    int i = 0;
    while(!p.isEnd())
    {
        char32_t c = p.getCharUTF();
        if(c != chrCodes[i++])
        {
            std::cout << "incorrect character code..." << std::endl;
            return false;
        }
        std::cout << c << ", ";
    }
    std::cout << std::endl;
    // Reset parser.
    p.reset();
    i = 0;
    while(!p.isEnd())
    {
        std::string word = p.getWordUTF();
        if(word != words[i++])
        {
            std::cout << "incorrect word..." << std::endl;
            return false;
        }
        std::cout << "Get Word: " << word << std::endl;
    }

    std::cout << "Convert UTF8 to UTF16" << std::endl;
    std::u16string utf16 = utf8to16(utf8);
    if(utf16 != wutf)
    {
        std::cout << "Conversion from UTF8 to UTF16 failed." << std::endl;
        return false;
    }
    std::cout << "UFT8 length: " << utf8.length() << " UTF16 length: " << utf16.length() << std::endl;
    std::cout << "Using UTF16 parser with u16string: " << utf16to8(utf16) << std::endl;
    StringParserUTF16 wp(utf16);
    std::cout << "Character codes: ";
    i = 0;
    while(!wp.isEnd())
    {
        char32_t c = wp.getCharUTF();
        if(c != chrCodes[i++])
        {
            std::cout << "incorrect character code..." << std::endl;
            return false;
        }
        std::cout << c << ", ";
    }
    std::cout << std::endl;
    // Reset the parser.
    wp.reset();
    i = 0;
    while(!wp.isEnd())
    {
        std::u16string word16 = wp.getWordUTF();
        std::string word = utf16to8(word16);
        if(word != words[i++])
        {
            std::cout << "incorrect word..." << std::endl;
            return false;
        }
        std::cout << "Get Word: " << word << std::endl;
    }

    std::cout << "String parser tests complete." << std::endl;
    std::cout << std::endl;
    return true;
}

//-------------------------------------------------------------------------
// UTF tests

bool testEscape()
{
    std::cout << "Starting escape tests." << std::endl;
    std::string unescaped = "Hello \n\"World!\"";
    std::cout << "Escaping: " << unescaped << std::endl;
    std::string escaped = escape(unescaped);
    std::cout << "Escaped: " << escaped << std::endl;
    unescaped = unescape(escaped);
    std::cout << "Unescaped: " << unescaped << std::endl;
    // XML test.
    escaped = "&quotThis is a &lttag&gt&quot";
    std::cout << "XML escaped: " << escaped << std::endl;
    unescaped = xmlUnescape(escaped);
    std::cout << "XML unescaped: " << unescaped << std::endl;
    escaped = xmlEscape(unescaped);
    std::cout << "XML escaped: " << escaped << std::endl;

    std::cout << "Starting UTF escape tests." << std::endl;

    std::string utf8 = u8"z\u6c34\U0001d10b Hello!";
    escaped = "z\\u6C34\\U0001D10B Hello!";
    unescaped = "z\346\260\264\360\235\204\213 Hello!";

    // perform escape sequence tests.
    std::cout << "UTF8: " << utf8 << std::endl;
    auto esc = utfEscape(utf8);
    if(esc != escaped)
    {
        std::cout << "UTF8 escaped sequence invalid." << std::endl;
        return false;
    }
    std::cout << "UTF8 escaped: " << esc << std::endl;
    esc = utfUnEscape(esc);
    if(esc != unescaped)
    {
        std::cout << "UTF8 unescaped sequence invalid." << std::endl;
        return false;
    }
    std::cout << "UTF8 unescaped: " << esc << std::endl;

    std::cout << "UTF escape tests complete." << std::endl;
    std::cout << std::endl;
    return true;
}

#endif /* STRINGTEST_H */

