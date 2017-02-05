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

#ifndef JSONDOCUMENT_H
#define JSONDOCUMENT_H

#include <mutex>
#include <vector>
#include <map>
#include <memory>
#include <sstream>

#include "../Parser/StringParser.h"
#include "../String/KString.h"

namespace KayLib
{

    enum JSONType
    {
        OBJECT, ARRAY, STRING, NUMBER, BOOL, _NULL
    };

    enum JSONError
    {
        NONE, UnexpectedEndOfDocument, InvalidSyntax, InvalidObjectName
    };

    std::string JSONErrorString(JSONError err)
    {
        switch(err)
        {
            case NONE:
                return "No error";
                break;
            case UnexpectedEndOfDocument:
                return "Parser reached the end of string without finishing the document";
                break;
            case InvalidSyntax:
                return "Invalid syntax";
                break;
            case InvalidObjectName:
                return "A JSONObject name was improperly formated";
                break;
        }
        return "Unknown error";
    }

    class JSONObject;
    class JSONArray;
    class JSONString;
    class JSONNumber;
    class JSONBool;
    class JSONNull;

    class JSONValue
    {
    public:
        /**
         * Get the type of value this object represents.
         * @return The value type.
         */
        virtual JSONType type() const = 0;

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        virtual JSONValue *copy() const = 0;

        /**
         * Return a formated string representation of the Value.
         * @param current The current indent.
         * @param indent The indention value to use.  The first line will NOT be indented.
         * @return The formated Value.
         */
        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const = 0;

        /**
         * Prints the value to an output stream without formating.
         * @param out The output stream.
         * @param value The value to output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream &out, const JSONValue& value)
        {
            value.format(out, "", "  ");
            return out;
        }

        /**
         * Check if this is a JSONObject.
         * @return True if this is a JSONObject.
         */
        inline bool isObject()
        {
            return (type() == JSONType::OBJECT);
        };

        /**
         * Check if this is a JSONArray.
         * @return True if this is a JSONArray.
         */
        inline bool isArray()
        {
            return (type() == JSONType::ARRAY);
        };

        /**
         * Check if this is a JSONString.
         * @return True if this is a JSONString.
         */
        inline bool isString()
        {
            return (type() == JSONType::STRING);
        };

        /**
         * Check if this is a JSONNumber.
         * @return True if this is a JSONNumber.
         */
        inline bool isNumber()
        {
            return (type() == JSONType::NUMBER);
        };

        /**
         * Check if this is a JSONBool.
         * @return True if this is a JSONBool.
         */
        inline bool isBool()
        {
            return (type() == JSONType::BOOL);
        };

        /**
         * Check if this is a JSONNull.
         * @return True if this is a JSONNull.
         */
        inline bool isNull()
        {
            return (type() == JSONType::_NULL);
        };

    protected:
        mutable std::mutex lockPtr;

        inline std::unique_lock<std::mutex> getLock() const
        {
            return std::unique_lock<std::mutex>(lockPtr);
        }
    };

    class JSONNull : public JSONValue
    {
    public:

        JSONNull() { };

        JSONNull(const JSONNull& orig) { };

        virtual ~JSONNull() { };

        virtual JSONType type() const override
        {
            return JSONType::_NULL;
        };

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        JSONValue *copy() const override
        {
            return new JSONNull();
        }

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override
        {
            out << "null";
        }

    };

    class JSONString : public JSONValue
    {
    public:

        JSONString(const std::string &nValue)
        {
            value = KString::escape(nValue, true);
        }

        JSONString(const JSONString& orig)
        {
            value = orig.value;
        }

        virtual ~JSONString() { }

        virtual JSONType type() const override
        {
            return JSONType::STRING;
        };

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        JSONValue *copy() const override
        {
            return new JSONString(*this);
        }

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override
        {
            std::unique_lock<std::mutex> uLock = getLock();
            out << "\"" << value << "\"";
        }

        /**
         * Get the value of this string.
         * @return The string.
         */
        std::string getValue() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return value;
        }

        /**
         * Set the value of this string.
         * @param nValue The value to set this string to.
         */
        void set(const std::string &nValue)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            value = KString::escape(nValue, true);
        }

    private:
        std::string value;
    };

    class JSONNumber : public JSONValue
    {
    public:

        JSONNumber(const long nValue)
        {
            _isDouble = false;
            lNumber = nValue;
            dNumber = (double) nValue;
        }

        JSONNumber(const double nValue)
        {
            _isDouble = true;
            lNumber = (long) nValue;
            dNumber = nValue;
            if(!std::isnormal(nValue))
            {
                lNumber = 0;
                dNumber = 0;
            }
        }

        JSONNumber(const JSONNumber& orig)
        {
            std::unique_lock<std::mutex> uLock = orig.getLock();
            _isDouble = orig._isDouble;
            lNumber = orig.lNumber;
            dNumber = orig.dNumber;
        }

        virtual ~JSONNumber() { }

        virtual JSONType type() const override
        {
            return JSONType::NUMBER;
        };

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        JSONValue *copy() const override
        {
            return new JSONNumber(*this);
        }

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override
        {
            std::unique_lock<std::mutex> uLock = getLock();
            if(_isDouble)
            {
                char buf[100];
                snprintf(buf, 100, "%.17g", dNumber);
                out << buf;
                //    out << std::to_string(dNumber);
            }
            else
            {
                out << std::to_string(lNumber);
            }
        }

        /**
         * Is this number a double?
         * @return True if it's a double false if it's an integer.
         */
        bool isDouble() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return _isDouble;
        }

        /**
         * The the integer representation of this value.
         * @return The integer value.
         */
        long getInt() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            if(_isDouble)
            {
                return (long) dNumber;
            }
            return lNumber;
        }

        /**
         * Get the double representation of this value.
         * @return The double value.
         */
        double getDouble() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            if(_isDouble)
            {
                return dNumber;
            }
            return (double) lNumber;
        }

        /**
         * Set the value of this number.
         * @param nValue The new value to set.
         */
        void set(long nValue)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            _isDouble = false;
            lNumber = nValue;
            dNumber = (double) nValue;
        }

        /**
         * Set the value of this number.
         * @param nValue The new value to set.
         */
        void set(double nValue)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            _isDouble = true;
            lNumber = (long) nValue;
            dNumber = nValue;
            if(!std::isnormal(nValue))
            {
                lNumber = 0;
                dNumber = 0;
            }
        }

    private:
        bool _isDouble;
        long lNumber;
        double dNumber;
    };

    class JSONBool : public JSONValue
    {
    public:

        JSONBool(const bool nValue)
        {
            value = nValue;
        }

        JSONBool(const JSONBool& orig)
        {
            std::unique_lock<std::mutex> uLock = orig.getLock();
            value = orig.value;
        }

        virtual ~JSONBool() { }

        virtual JSONType type() const override
        {
            return JSONType::BOOL;
        };

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        JSONValue *copy() const override
        {
            return new JSONBool(*this);
        }

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override
        {
            std::unique_lock<std::mutex> uLock = getLock();
            out << (value ? "true" : "false");
        }

        /**
         * Get the value of this bool.
         * @return The value.
         */
        bool get() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return value;
        };

        /**
         * Set the value of this bool.
         * @param nValue The new value.
         */
        void set(bool nValue)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            value = nValue;
        }

    private:
        bool value;
    };

    class JSONObject : public JSONValue
    {
    public:

        JSONObject() { }

        JSONObject(const JSONObject& orig)
        {
            std::unique_lock<std::mutex> uLock = orig.getLock();
            for(auto value : orig.values)
            {
                if(value.second)
                {
                    values[value.first] = std::shared_ptr<JSONValue>(value.second->copy());
                }
            }
        }

        virtual ~JSONObject() { }

        virtual JSONType type() const override
        {
            return JSONType::OBJECT;
        };

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        JSONValue *copy() const override
        {
            return new JSONObject(*this);
        }

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override
        {
            out << "{" << std::endl;
            bool first = true;
            std::unique_lock<std::mutex> uLock = getLock();
            for(auto value : values)
            {
                if(!first)
                {
                    out << "," << std::endl;
                }
                out << current + indent << "\"" << value.first << "\" : ";
                value.second->format(out, current + indent, indent);
                first = false;
            }
            out << std::endl << current + "}";
        }

        /**
         * Get the names of all values of this element.
         * @return The list of value names.
         */
        std::vector<std::string> getValueNames() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            std::vector<std::string> names;
            for(auto entry : values)
            {
                names.push_back(entry.first);
            }
            return names;
        }

        /**
         * Check if the named value exists.
         * @param valName The value to check for.
         * @return True if the value exists. False if not.
         */
        bool hasValue(const std::string &valName) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return values.find(valName) != values.end();
        }

        /**
         * Get the named value.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONValue> getValue(const std::string &valName) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            auto itr = values.find(valName);
            if(itr == values.end())
            {
                return nullptr;
            }
            return itr->second;
        }

        /**
         * Get the named value if it is a JSONString.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONString> getValueAsString(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return nullptr;
            }
            if(val->type() != JSONType::STRING)
            {
                return nullptr;
            }
            return std::dynamic_pointer_cast<JSONString>(val);
        }

        /**
         * Get the named value if it is a JSONNumber.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONNumber> getValueAsNumber(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return nullptr;
            }
            if(val->type() != JSONType::NUMBER)
            {
                return nullptr;
            }
            return std::dynamic_pointer_cast<JSONNumber>(val);
        }

        /**
         * Get the named value if it is a JSONBool.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONBool> getValueAsBool(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return nullptr;
            }
            if(val->type() != JSONType::BOOL)
            {
                return nullptr;
            }
            return std::dynamic_pointer_cast<JSONBool>(val);
        }

        /**
         * Get the named value if it is a JSONNull.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONNull> getValueAsNull(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return nullptr;
            }
            if(val->type() != JSONType::_NULL)
            {
                return nullptr;
            }
            return std::dynamic_pointer_cast<JSONNull>(val);
        }

        /**
         * Get the named value if it is a JSONObject.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONObject> getValueAsObject(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return nullptr;
            }
            if(val->type() != JSONType::OBJECT)
            {
                return nullptr;
            }
            return std::dynamic_pointer_cast<JSONObject>(val);
        }

        /**
         * Get the named value if it is a JSONArray.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONArray> getValueAsArray(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return nullptr;
            }
            if(val->type() != JSONType::ARRAY)
            {
                return nullptr;
            }
            return std::dynamic_pointer_cast<JSONArray>(val);
        }

        /**
         * Get the named value if it can be represented as a string.
         * @param valName The value to get.
         * @return The value or an empty string if not found.
         */
        std::string getString(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return "";
            }
            if(val->type() == JSONType::STRING)
            {
                // A string return the value.
                std::shared_ptr<JSONString> str = std::dynamic_pointer_cast<JSONString>(val);
                return str->getValue();
            }
            if(val->type() == JSONType::NUMBER)
            {
                // It's a number convert it to a string.
                std::shared_ptr<JSONNumber> num = std::dynamic_pointer_cast<JSONNumber>(val);
                if(num->isDouble())
                {
                    return std::to_string(num->getDouble());
                }
                return std::to_string(num->getInt());
            }
            if(val->type() == JSONType::BOOL)
            {
                std::shared_ptr<JSONBool> bl = std::dynamic_pointer_cast<JSONBool>(val);
                // It's a bool return true/false
                if(bl->get())
                {
                    return "true";
                }
                return "false";
            }
            if(val->type() == JSONType::_NULL)
            {
                return "null";
            }
            // Objects and arrays cannot be converted to strings.
            return "";
        }

        /**
         * Get the named value if it can be represented as a int.
         * @param valName The value to get.
         * @return The value or zero if not found.
         * @note a value of zero does not guarantee that it is a valid result.
         */
        long getInt(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return 0;
            }
            if(val->type() == JSONType::STRING)
            {
                std::shared_ptr<JSONString> str = std::dynamic_pointer_cast<JSONString>(val);
                // A string attempt to parse the value.
                StringParser<char> parser(str->getValue());
                if(parser.isDecimal())
                {
                    return (long) parser.getDouble();
                }
                return parser.getLong();
            }
            if(val->type() == JSONType::NUMBER)
            {
                std::shared_ptr<JSONNumber> num = std::dynamic_pointer_cast<JSONNumber>(val);
                // It's a number just return the value.
                return num->getInt();
            }
            if(val->type() == JSONType::BOOL)
            {
                std::shared_ptr<JSONBool> bl = std::dynamic_pointer_cast<JSONBool>(val);
                // It's a bool return 1/0
                if(bl->get())
                {
                    return 1;
                }
                return 0;
            }
            if(val->type() == JSONType::_NULL)
            {
                // It's null return 0.
                return 0;
            }
            // Objects and arrays cannot be converted to numbers.
            return 0;
        }

        /**
         * Get the named value if it can be represented as a double.
         * @param valName The value to get.
         * @return The value or zero if not found.
         * @note a value of zero does not guarantee that it is a valid result.
         */
        double getDouble(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return 0;
            }
            if(val->type() == JSONType::STRING)
            {
                std::shared_ptr<JSONString> str = std::dynamic_pointer_cast<JSONString>(val);
                // A string attempt to parse the value.
                StringParser<char> parser(str->getValue());
                return parser.getDouble();
            }
            if(val->type() == JSONType::NUMBER)
            {
                std::shared_ptr<JSONNumber> num = std::dynamic_pointer_cast<JSONNumber>(val);
                // It's a number just return the value.
                return num->getDouble();
            }
            if(val->type() == JSONType::BOOL)
            {
                std::shared_ptr<JSONBool> bl = std::dynamic_pointer_cast<JSONBool>(val);
                // It's a bool return 1/0
                if(bl->get())
                {
                    return 1;
                }
                return 0;
            }
            if(val->type() == JSONType::_NULL)
            {
                // It's null return 0.
                return 0;
            }
            // Objects and arrays cannot be converted to numbers.
            return 0;
        }

        /**
         * Get the named value if it can be represented as a bool.
         * @param valName The value to get.
         * @return The 0 if false, 1 if true or -1 if not found.
         */
        int getBool(const std::string &valName) const
        {
            std::shared_ptr<JSONValue> val = getValue(valName);
            if(val.get() == nullptr)
            {
                return -1;
            }
            if(val->type() == JSONType::STRING)
            {
                std::shared_ptr<JSONString> str = std::dynamic_pointer_cast<JSONString>(val);
                // A string attempt to parse the value.
                StringParser<char> parser(str->getValue());
                if(parser.nextIs("true"))
                {
                    return true;
                }
                if(parser.nextIs("false"))
                {
                    return false;
                }
                return -1;
            }
            if(val->type() == JSONType::NUMBER)
            {
                std::shared_ptr<JSONNumber> num = std::dynamic_pointer_cast<JSONNumber>(val);
                if(num->isDouble())
                {
                    return num->getDouble() != 0;
                }
                return num->getInt() != 0;
            }
            if(val->type() == JSONType::BOOL)
            {
                std::shared_ptr<JSONBool> bl = std::dynamic_pointer_cast<JSONBool>(val);
                // It's a bool return the value.
                return bl->get();
            }
            if(val->type() == JSONType::_NULL)
            {
                // It's null return 0.
                return false;
            }
            // Objects and arrays cannot be converted to bool.
            return -1;
        }

        /**
         * Set a value for the object.
         * @param name The name of the value.
         * @param value The value.
         */
        void setValue(const std::string &name, JSONValue *value)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            values[name] = std::shared_ptr<JSONValue>(value);
        }

        /**
         * Set a value for the object.
         * @param name The name of the value.
         * @param value The value.
         */
        void setValue(const std::string &name, std::shared_ptr<JSONValue> value)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            values[name] = value;
        }

    private:
        std::map<std::string, std::shared_ptr<JSONValue>> values;
    };

    class JSONArray : public JSONValue
    {
    public:

        JSONArray() { }

        JSONArray(const JSONArray& orig)
        {
            std::unique_lock<std::mutex> uLock = orig.getLock();
            for(auto value : orig.values)
            {
                if(value)
                {
                    values.push_back(std::shared_ptr<JSONValue>(value->copy()));
                }
            }
        }

        virtual ~JSONArray() { }

        virtual JSONType type() const override
        {
            return JSONType::ARRAY;
        };

        /**
         * Create a copy of the value.
         * @return The copy.
         */
        JSONValue *copy() const override
        {
            return new JSONArray(*this);
        }

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override
        {
            out << "[" << std::endl;
            bool first = true;
            std::unique_lock<std::mutex> uLock = getLock();
            for(auto value : values)
            {
                if(!first)
                {
                    out << "," << std::endl;
                }
                out << current + indent;
                value->format(out, current + indent, indent);
                first = false;
            }
            out << std::endl << current + "]";
        }

        /**
         * Get the array of values.
         * @return The array values.
         */
        std::vector<std::shared_ptr<JSONValue>> getArray() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return std::vector<std::shared_ptr < JSONValue >> (values);
        }

        /**
         * Add another value to the array.
         * @param add The value to add.
         */
        void add(const std::shared_ptr<JSONValue> &add)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            values.push_back(add);
        }

    private:
        std::vector<std::shared_ptr<JSONValue>> values;
    };

    class JSONDocument
    {
    public:

        JSONDocument()
        {
            resetError();
            root = std::make_shared<JSONObject>();
        }

        /**
         * Create a new JSONDocument from the string.
         * @param doc The string containing the document to parse.
         */
        JSONDocument(const std::string &doc)
        {
            resetError();
            root = parse(doc);
        }

        JSONDocument(const JSONDocument& orig)
        {
            resetError();
            if(root)
            {
                root.reset(root->copy());
            }
        }

        virtual ~JSONDocument() { }

        /**
         * Get the documents root element.
         * @return The root element.
         */
        std::shared_ptr<JSONValue> getRoot() const
        {
            return root;
        };

        /**
         * Return a formated string representation of the document.
         * @param indent The indention value to use.
         * @return The formated document.
         */
        std::string format(const std::string &indent) const
        {
            std::stringstream out;
            if(root.get() != NULL)
            {
                root->format(out, "", indent);
            }
            return out.str();
        }

        /**
         * Prints the document to an output stream without formating.
         * @param out The output stream.
         * @param doc The document to output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream &out, const JSONDocument& doc)
        {
            if(doc.root.get())
            {
                out << *doc.root.get();
            }
            return out;
        }

        /**
         * Get the last error code.
         * @return The error code.
         */
        JSONError getError()
        {
            return lastError;
        }

        /**
         * Get the parser index of the last error.
         * @return The index of the error.
         */
        int getErrorIndex()
        {
            return errorIndex;
        }

        /**
         * Reset the error code.
         */
        void resetError()
        {
            lastError = NONE;
            errorIndex = -1;
        }

    private:
        std::shared_ptr<JSONValue> root;
        JSONError lastError = NONE;
        int errorIndex = -1;

        std::shared_ptr<JSONValue> parse(const std::string &doc)
        {
            StringParser<char> parser(doc);
            return parse(parser);
        }

        std::shared_ptr<JSONValue> parse(StringParser<char> &parser)
        {
            parser.skipWhitespace(true);
            if(parser.peekChar() == '{')
            {
                return parseObject(parser);
            }
            if(parser.peekChar() == '[')
            {
                return parseArray(parser);
            }
            if(parser.peekChar() == '\"')
            {
                // A string.
                std::string s = parser.getQuotedString();
                return std::make_shared<JSONString>(s);
            }
            if(parser.peekChar() == '-' || parser.isDigit())
            {
                // A number.
                if(parser.isDecimal())
                {
                    return std::make_shared<JSONNumber>(parser.getDouble());
                }
                return std::make_shared<JSONNumber>(parser.getLong());
            }
            if(parser.nextIs("true", true))
            {
                // A true bool.
                return std::make_shared<JSONBool>(true);
            }
            if(parser.nextIs("false", true))
            {
                // A false bool.
                return std::make_shared<JSONBool>(false);
            }
            if(parser.nextIs("null", true))
            {
                // A null.
                return std::make_shared<JSONNull>();
            }
            // Set error.
            errorIndex = parser.getIndex();
            lastError = InvalidSyntax;
            return std::shared_ptr<JSONValue>();
        }

        std::shared_ptr<JSONValue> parseObject(StringParser<char> &parser)
        {
            parser.getChar();
            // An object.
            std::shared_ptr<JSONObject> object = std::make_shared<JSONObject>();
            // Skip whitespace.
            parser.skipWhitespace(true);
            if(parser.peekChar() == '}')
            {
                // An empty object.
                parser.getChar();
                return object;
            }
            char last = ' ';
            do
            {
                // Skip whitespace.
                parser.skipWhitespace(true);
                if(parser.peekChar() != '\"')
                {
                    // Set error.
                    errorIndex = parser.getIndex();
                    lastError = InvalidObjectName;
                    // There was an error, clear and return.
                    object.reset();
                    return object;
                }
                // Get value name.
                std::string name = parser.getQuotedString();
                // Skip whitespace.
                parser.skipWhitespace(true);
                // name and value must be separated by a colon.
                if(parser.getChar() != ':')
                {
                    // Set error.
                    errorIndex = parser.getIndex();
                    lastError = InvalidSyntax;
                    // There was an error, clear and return.
                    object.reset();
                    return object;
                }
                // get the entries.
                std::shared_ptr<JSONValue> child = parse(parser);
                if(lastError != NONE)
                {
                    // There was an error, clear and return.
                    object.reset();
                    return object;
                }
                object->setValue(name, child);
                // Skip whitespace.
                parser.skipWhitespace(true);
            }
            while((last = parser.getChar()) == ',');
            // check that we are at the end of the array.
            if(last != '}')
            {
                // Set error.
                errorIndex = parser.getIndex();
                lastError = InvalidSyntax;
                // There was an error, clear and return.
                object.reset();
                return object;
            }
            return object;
        }

        std::shared_ptr<JSONValue> parseArray(StringParser<char> &parser)
        {
            parser.getChar();
            // An array.
            std::shared_ptr<JSONArray> array = std::make_shared<JSONArray>();
            // Skip whitespace.
            parser.skipWhitespace(true);
            if(parser.peekChar() == ']')
            {
                // An empty array.
                parser.getChar();
                return array;
            }
            char last = ' ';
            do
            {
                // get the entries.
                std::shared_ptr<JSONValue> child = parse(parser);
                if(lastError != NONE)
                {
                    // There was an error, clear and return.
                    array.reset();
                    return array;
                }
                array->add(child);
                // Skip whitespace.
                parser.skipWhitespace(true);
            }
            while((last = parser.getChar()) == ',');
            // check that we are at the end of the array.
            if(last != ']')
            {
                // Set error.
                errorIndex = parser.getIndex();
                lastError = InvalidSyntax;
                // There was an error, clear and return.
                array.reset();
                return array;
            }
            return array;
        }
    };

}

#endif /* JSONDOCUMENT_H */

