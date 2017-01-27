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

#include "JSON.h"
#include "../Parser/StringParser.h"
#include "../IO/Exceptions.h"
#include "../String/KString.h"

std::string quote = "\"";

namespace KayLib
{

    JSONValue *JSONValue::copy(const JSONValue &value)
    {
        return copy(&value);
    };

    JSONValue *JSONValue::copy(const JSONValue *value)
    {
        switch(value->type())
        {
            case JSONType::OBJECT:
                return new JSONObject((JSONObject &) value);
            case JSONType::ARRAY:
                return new JSONArray((JSONArray &) value);
            case JSONType::BOOL:
                return new JSONBool((JSONBool &) value);
            case JSONType::NUMBER:
                return new JSONNumber((JSONNumber &) value);
            case JSONType::STRING:
                return new JSONString((JSONString &) value);
            case JSONType::_NULL:
                return new JSONNull((JSONNull &) value);
        }
        return new JSONNull();
    }

    JSONValue *JSONValue::copy(const std::shared_ptr<JSONValue> &value)
    {
        return copy(value.get());
    }

    JSONValue *parse(const std::string &doc);
    JSONValue *parse(StringParser<char> &parser);
    JSONValue *parseObject(StringParser<char> &parser);
    JSONValue *parseArray(StringParser<char> &parser);

    JSONValue *parse(const std::string &doc)
    {
        StringParser<char> parser(doc);
        return parse(parser);
    }

    JSONValue *parseObject(StringParser<char> &parser)
    {
        parser.getChar();
        // An object.
        JSONObject *object = new JSONObject();
        // Skip whitespace.
        parser.skipWhitespace(true);
        if(parser.peekChar() == '}')
        {
            // An empty object.
            parser.getChar();
            return object;
        }
        char last = ' ';
        try
        {
            do
            {
                // Skip whitespace.
                parser.skipWhitespace(true);
                if(parser.peekChar() != '\"')
                {
                    throw new ParserException("Invalid JSONObject name", parser.getParseString(), parser.getIndex());
                }
                // Get value name.
                std::string name = parser.getQuotedString();
                // Skip whitespace.
                parser.skipWhitespace(true);
                // name and value must be separated by a colon.
                if(parser.getChar() != ':')
                {
                    throw new ParserException("Invalid JSONObject", parser.getParseString(), parser.getIndex());
                }
                // get the entries.
                object->setValue(name, parse(parser));
                // Skip whitespace.
                parser.skipWhitespace(true);
            }
            while((last = parser.getChar()) == ',');
        }
        catch(ParserException *pErr)
        {
            // Delete the object to prevent memory leaks then re-throw the exception.
            delete object;
            throw;
        }
        // check that we are at the end of the array.
        if(last != '}')
        {
            // Delete the object to prevent memory leaks then throw the exception.
            delete object;
            throw new ParserException("Invalid JSONObject", parser.getParseString(), parser.getIndex());
        }
        return object;
    }

    JSONValue *parseArray(StringParser<char> &parser)
    {
        parser.getChar();
        // An array.
        JSONArray *array = new JSONArray();
        // Skip whitespace.
        parser.skipWhitespace(true);
        if(parser.peekChar() == ']')
        {
            // An empty array.
            parser.getChar();
            return array;
        }
        char last = ' ';
        try
        {
            do
            {
                // get the entries.
                array->add(std::shared_ptr<JSONValue>(parse(parser)));
                // Skip whitespace.
                parser.skipWhitespace(true);
            }
            while((last = parser.getChar()) == ',');
        }
        catch(ParserException *pErr)
        {
            // Delete the array to prevent memory leaks then re-throw the exception.
            delete array;
            throw;
        }
        // check that we are at the end of the array.
        if(last != ']')
        {
            // Delete the array to prevent memory leaks then throw the exception.
            delete array;
            throw new ParserException("Invalid JSONArray", parser.getParseString(), parser.getIndex());
        }
        return array;
    }

    JSONValue *parse(StringParser<char> &parser)
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
            return new JSONString(parser.getQuotedString());
        }
        if(parser.peekChar() == '-' || parser.isDigit())
        {
            // A number.
            if(parser.isDecimal())
            {
                return new JSONNumber(parser.getDouble());
            }
            return new JSONNumber(parser.getLong());
        }
        if(parser.nextIs("true", true))
        {
            // A true bool.
            return new JSONBool(true);
        }
        if(parser.nextIs("false", true))
        {
            // A false bool.
            return new JSONBool(false);
        }
        if(parser.nextIs("null", true))
        {
            // A null.
            return new JSONNull();
        }
        throw new ParserException("Invalid JSON", parser.getParseString(), parser.getIndex());
    }

    JSONObject::JSONObject() { }

    JSONObject::JSONObject(const JSONObject& orig)
    {
        std::unique_lock<std::mutex> uLock = orig.getLock();
        for(auto value : orig.values)
        {
            values[value.first] = std::shared_ptr<JSONValue>(JSONValue::copy(value.second));
        }
    }

    JSONObject::~JSONObject() { }

    std::vector<std::string> JSONObject::getValueNames() const
    {
        std::unique_lock<std::mutex> uLock = getLock();
        std::vector<std::string> names;
        for(auto entry : values)
        {
            names.push_back(entry.first);
        }
        return names;
    }

    bool JSONObject::hasValue(const std::string &valName) const
    {
        std::unique_lock<std::mutex> uLock = getLock();
        return values.find(valName) != values.end();
    }

    std::shared_ptr<JSONValue> JSONObject::getValue(const std::string &valName) const
    {
        std::unique_lock<std::mutex> uLock = getLock();
        auto itr = values.find(valName);
        if(itr == values.end())
        {
            return nullptr;
        }
        return itr->second;
    }

    std::shared_ptr<JSONString> JSONObject::getValueAsString(const std::string &valName) const
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
        return std::shared_ptr<JSONString>(val->asString());
    }

    std::shared_ptr<JSONNumber> JSONObject::getValueAsNumber(const std::string &valName) const
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
        return std::shared_ptr<JSONNumber>(val->asNumber());
    }

    std::shared_ptr<JSONBool> JSONObject::getValueAsBool(const std::string &valName) const
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
        return std::shared_ptr<JSONBool>(val->asBool());
    }

    std::shared_ptr<JSONNull> JSONObject::getValueAsNull(const std::string &valName) const
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
        return std::shared_ptr<JSONNull>(val->asNull());
    }

    std::shared_ptr<JSONObject> JSONObject::getValueAsObject(const std::string &valName) const
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
        return std::shared_ptr<JSONObject>(val->asObject());
    }

    std::shared_ptr<JSONArray> JSONObject::getValueAsArray(const std::string &valName) const
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
        return std::shared_ptr<JSONArray>(val->asArray());
    }

    std::string JSONObject::getString(const std::string &valName) const
    {
        std::shared_ptr<JSONValue> val = getValue(valName);
        if(val.get() == nullptr)
        {
            return "";
        }
        if(val->type() == JSONType::STRING)
        {
            // A string return the value.
            return val->asString()->getValue();
        }
        if(val->type() == JSONType::NUMBER)
        {
            // It's a number convert it to a string.
            JSONNumber *num = val->asNumber();
            if(num->isDouble())
            {
                return std::to_string(num->getDouble());
            }
            return std::to_string(num->getInt());
        }
        if(val->type() == JSONType::BOOL)
        {
            // It's a bool return true/false
            if(val->asBool()->get())
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

    double JSONObject::getDouble(const std::string &valName) const
    {
        std::shared_ptr<JSONValue> val = getValue(valName);
        if(val.get() == nullptr)
        {
            return 0;
        }
        if(val->type() == JSONType::STRING)
        {
            // A string attempt to parse the value.
            StringParser<char> parser(val->asString()->getValue());
            return parser.getDouble();
        }
        if(val->type() == JSONType::NUMBER)
        {
            // It's a number just return the value.
            return val->asNumber()->getDouble();
        }
        if(val->type() == JSONType::BOOL)
        {
            // It's a bool return 1/0
            if(val->asBool()->get())
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

    long JSONObject::getInt(const std::string &valName) const
    {
        std::shared_ptr<JSONValue> val = getValue(valName);
        if(val.get() == nullptr)
        {
            return 0;
        }
        if(val->type() == JSONType::STRING)
        {
            // A string attempt to parse the value.
            StringParser<char> parser(val->asString()->getValue());
            if(parser.isDecimal())
            {
                return (long) parser.getDouble();
            }
            return parser.getLong();
        }
        if(val->type() == JSONType::NUMBER)
        {
            // It's a number just return the value.
            return val->asNumber()->getInt();
        }
        if(val->type() == JSONType::BOOL)
        {
            // It's a bool return 1/0
            if(val->asBool()->get())
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

    int JSONObject::getBool(const std::string &valName) const
    {
        std::shared_ptr<JSONValue> val = getValue(valName);
        if(val.get() == nullptr)
        {
            return -1;
        }
        if(val->type() == JSONType::STRING)
        {
            // A string attempt to parse the value.
            StringParser<char> parser(val->asString()->getValue());
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
            JSONNumber *num = val->asNumber();
            if(num->isDouble())
            {
                return num->getDouble() != 0;
            }
            return num->getInt() != 0;
        }
        if(val->type() == JSONType::BOOL)
        {
            // It's a bool return the value.
            return val->asBool()->get();
        }
        if(val->type() == JSONType::_NULL)
        {
            // It's null return 0.
            return false;
        }
        // Objects and arrays cannot be converted to bool.
        return -1;
    }

    void JSONObject::setValue(const std::string &name, JSONValue *value)
    {
        std::unique_lock<std::mutex> uLock = getLock();
        values[name] = std::shared_ptr<JSONValue>(value);
    }

    JSONString::JSONString(const std::string &nValue)
    {
        value = escape(nValue, true);
    }

    JSONString::JSONString(const JSONString& orig)
    {
        value = orig.getValue();
    }

    JSONString::~JSONString() { }

    void JSONString::set(const std::string &nValue)
    {
        std::unique_lock<std::mutex> uLock = getLock();
        value = escape(nValue, true);
    }

    JSONNumber::JSONNumber(const long nValue)
    {
        _isDouble = false;
        lNumber = nValue;
        dNumber = (double) nValue;
    }

    JSONNumber::JSONNumber(const double nValue)
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

    JSONNumber::JSONNumber(const JSONNumber& orig)
    {
        std::unique_lock<std::mutex> uLock = orig.getLock();
        _isDouble = orig._isDouble;
        lNumber = orig.lNumber;
        dNumber = orig.dNumber;
    }

    JSONNumber::~JSONNumber() { }

    void JSONNumber::set(long nValue)
    {
        std::unique_lock<std::mutex> uLock = getLock();
        _isDouble = false;
        lNumber = nValue;
        dNumber = (double) nValue;
    }

    void JSONNumber::set(double nValue)
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

    JSONBool::JSONBool(const bool nValue)
    {
        value = nValue;
    }

    JSONBool::JSONBool(const JSONBool& orig)
    {
        std::unique_lock<std::mutex> uLock = orig.getLock();
        value = orig.value;
    }

    JSONBool::~JSONBool() { }

    void JSONBool::set(bool nValue)
    {
        std::unique_lock<std::mutex> uLock = getLock();
        value = nValue;
    }

    JSONArray::JSONArray() { }

    JSONArray::JSONArray(const JSONArray& orig)
    {
        std::unique_lock<std::mutex> uLock = orig.getLock();
        for(auto value : orig.values)
        {
            values.push_back(std::shared_ptr<JSONValue>(JSONValue::copy(value)));
        }
    }

    JSONArray::~JSONArray() { }

    std::vector<std::shared_ptr<JSONValue>> JSONArray::getArray() const
    {
        std::unique_lock<std::mutex> uLock = getLock();
        return std::vector<std::shared_ptr < JSONValue >> (values);
    }

    void JSONArray::add(const std::shared_ptr<JSONValue> &add)
    {
        std::unique_lock<std::mutex> uLock = getLock();
        values.push_back(add);
    }

    JSONDocument::JSONDocument() { }

    JSONDocument::JSONDocument(const std::string &doc)
    {
        root.reset(parse(doc));
    }

    JSONDocument::JSONDocument(const JSONDocument& orig)
    {
        root.reset(JSONValue::copy(root.get()));
    }

    JSONDocument::~JSONDocument() { }

    std::string JSONDocument::format(const std::string &indent) const
    {
        std::stringstream out;
        if(root.get() != NULL)
        {
            root->format(out, "", indent);
        }
        return out.str();
    }

    void JSONNull::format(std::ostream &out, const std::string &current, const std::string &indent) const
    {
        out << "null";
    }

    void JSONObject::format(std::ostream &out, const std::string &current, const std::string &indent) const
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
            out << current + indent << quote << value.first << "\" : ";
            value.second->format(out, current + indent, indent);
            first = false;
        }
        out << std::endl << current + "}";
    }

    void JSONString::format(std::ostream &out, const std::string &current, const std::string &indent) const
    {
        std::unique_lock<std::mutex> uLock = getLock();
        out << quote << value << quote;
    }

    void JSONNumber::format(std::ostream &out, const std::string &current, const std::string &indent) const
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

    void JSONBool::format(std::ostream &out, const std::string &current, const std::string &indent) const
    {
        std::unique_lock<std::mutex> uLock = getLock();
        out << (value ? "true" : "false");
    }

    void JSONArray::format(std::ostream &out, const std::string &current, const std::string &indent) const
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

    std::ostream& operator<<(std::ostream &out, const JSONValue& value)
    {
        value.format(out, "", "  ");
        return out;
    }

    std::ostream& operator<<(std::ostream &out, const JSONDocument& doc)
    {
        if(doc.root.get() != nullptr)
        {
            out << *doc.root.get();
        }
        return out;
    }

}
