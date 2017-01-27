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

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

namespace KayLib
{

    class ValueNotFoundException : public std::exception
    {
    public:

        ValueNotFoundException(const std::string valueName)
        {
            value = valueName;
        }

        virtual const char *what() const throw ()
        {
            return ("Value " + value + " not found.").c_str();
        };

        std::string valueName()
        {
            return value;
        };

    private:
        std::string value;
    };

    class AttributeNotFoundException : public std::exception
    {
    public:

        AttributeNotFoundException(const std::string attributeName)
        {
            attribute = attributeName;
        }

        virtual const char *what() const throw ()
        {
            return ("Attribute " + attribute + " not found.").c_str();
        };

        std::string attributeName()
        {
            return attribute;
        };

    private:
        std::string attribute;
    };

    class ParserException : public std::exception
    {
    public:

        ParserException(const std::string error, const std::string parse, int index)
        {
            parseError = error;
            parseIndex = index;
            stringStart = index - 20;
            if(stringStart < 0)
            {
                stringStart = 0;
            }
            parseString = parse.substr(stringStart, 60);
        }

        virtual const char *what() const throw ()
        {
            return (parseError + ": Error parsing string \"" + parseString + "\" at index" + std::to_string(parseIndex)).c_str();
        };

        /**
         * Get the error description.
         * @return The error description.
         */
        std::string getError()
        {
            return parseError;
        };

        /**
         * Get the fragment of the parser string where the error occurred.
         * @return The string fragment.
         */
        std::string getString()
        {
            return parseString;
        };

        /**
         * Get the index into the original parser string that the error occurred.
         * @return The index the error occurred.
         */
        int getIndex()
        {
            return parseIndex;
        }

        /**
         * Get the index of the start of the kept parse string.
         * @return The index that the parser fragment started at.
         */
        int getStringStart()
        {
            return stringStart;
        }

    private:
        std::string parseError;
        std::string parseString;
        int parseIndex;
        int stringStart;
    };

}

#endif /* EXCEPTIONS_H */

