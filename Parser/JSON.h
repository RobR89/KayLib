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

namespace KayLib
{

    enum JSONType
    {
        OBJECT, ARRAY, STRING, NUMBER, BOOL, _NULL
    };

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
         * @param value The value to copy.
         * @return The copy.
         */
        static JSONValue *copy(const JSONValue &value);
        /**
         * Create a copy of the value.
         * @param value The value to copy.
         * @return The copy.
         */
        static JSONValue *copy(const JSONValue *value);
        /**
         * Create a copy of the value.
         * @param value The value to copy.
         * @return The copy.
         */
        static JSONValue *copy(const std::shared_ptr<JSONValue> &value);

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
        friend std::ostream& operator<<(std::ostream &out, const JSONValue& value);

        /**
         * Return the value as a JSONObject or nullptr if the value is not a JSONObject.
         * @return The value as a JSONObject.
         */
        inline JSONObject *asObject()
        {
            if(type() == JSONType::OBJECT)
            {
                return (JSONObject *)this;
            }
            return nullptr;
        };

        /**
         * Return the value as a JSONArray or nullptr if the value is not a JSONArray.
         * @return The value as a JSONArray.
         */
        inline JSONArray *asArray()
        {
            if(type() == JSONType::ARRAY)
            {
                return (JSONArray *)this;
            }
            return nullptr;
        };

        /**
         * Return the value as a JSONString or nullptr if the value is not a JSONString.
         * @return The value as a JSONString.
         */
        inline JSONString *asString()
        {
            if(type() == JSONType::STRING)
            {
                return (JSONString *)this;
            }
            return nullptr;
        };

        /**
         * Return the value as a JSONNumber or nullptr if the value is not a JSONNumber.
         * @return The value as a JSONNumber.
         */
        inline JSONNumber *asNumber()
        {
            if(type() == JSONType::NUMBER)
            {
                return (JSONNumber *)this;
            }
            return nullptr;
        };

        /**
         * Return the value as a JSONBool or nullptr if the value is not a JSONBool.
         * @return The value as a JSONBool.
         */
        inline JSONBool *asBool()
        {
            if(type() == JSONType::BOOL)
            {
                return (JSONBool *)this;
            }
            return nullptr;
        };

        /**
         * Return the value as a JSONNull or nullptr if the value is not a JSONNull.
         * @return The value as a JSONNull.
         */
        inline JSONNull *asNull()
        {
            if(type() == JSONType::_NULL)
            {
                return (JSONNull *)this;
            }
            return nullptr;
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

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override;

    };

    class JSONObject : public JSONValue
    {
    public:
        JSONObject();
        JSONObject(const JSONObject& orig);
        virtual ~JSONObject();

        virtual JSONType type() const override
        {
            return JSONType::OBJECT;
        };

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override;

        /**
         * Get the names of all values of this element.
         * @return The list of value names.
         */
        std::vector<std::string> getValueNames() const;

        /**
         * Check if the named value exists.
         * @param valName The value to check for.
         * @return True if the value exists. False if not.
         */
        bool hasValue(const std::string &valName) const;

        /**
         * Get the named value.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONValue> getValue(const std::string &valName) const;

        /**
         * Get the named value if it is a JSONString.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONString> getValueAsString(const std::string &valName) const;

        /**
         * Get the named value if it is a JSONNumber.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONNumber> getValueAsNumber(const std::string &valName) const;

        /**
         * Get the named value if it is a JSONBool.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONBool> getValueAsBool(const std::string &valName) const;

        /**
         * Get the named value if it is a JSONNull.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONNull> getValueAsNull(const std::string &valName) const;

        /**
         * Get the named value if it is a JSONObject.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONObject> getValueAsObject(const std::string &valName) const;

        /**
         * Get the named value if it is a JSONArray.
         * @param valName The value to get.
         * @return The value or nullptr if not found.
         */
        std::shared_ptr<JSONArray> getValueAsArray(const std::string &valName) const;

        /**
         * Get the named value if it can be represented as a string.
         * @param valName The value to get.
         * @return The value or an empty string if not found.
         */
        std::string getString(const std::string &valName) const;

        /**
         * Get the named value if it can be represented as a int.
         * @param valName The value to get.
         * @return The value or zero if not found.
         * @note a value of zero does not guarantee that it is a valid result.
         */
        long getInt(const std::string &valName) const;

        /**
         * Get the named value if it can be represented as a double.
         * @param valName The value to get.
         * @return The value or zero if not found.
         * @note a value of zero does not guarantee that it is a valid result.
         */
        double getDouble(const std::string &valName) const;

        /**
         * Get the named value if it can be represented as a bool.
         * @param valName The value to get.
         * @return The 0 if false, 1 if true or -1 if not found.
         */
        int getBool(const std::string &valName) const;


        /**
         * Set a value for the object.
         * @param name The name of the value.
         * @param value The value.
         */
        void setValue(const std::string &name, JSONValue *value);

    private:
        std::map<std::string, std::shared_ptr<JSONValue>> values;
    };

    class JSONString : public JSONValue
    {
    public:
        JSONString(const std::string &nValue);
        JSONString(const JSONString& orig);
        virtual ~JSONString();

        virtual JSONType type() const override
        {
            return JSONType::STRING;
        };

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override;

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
        void set(const std::string &nValue);

    private:
        std::string value;
    };

    class JSONNumber : public JSONValue
    {
    public:
        JSONNumber(const long nValue);
        JSONNumber(const double nValue);
        JSONNumber(const JSONNumber& orig);
        virtual ~JSONNumber();

        virtual JSONType type() const override
        {
            return JSONType::NUMBER;
        };

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override;

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
        void set(long nValue);

        /**
         * Set the value of this number.
         * @param nValue The new value to set.
         */
        void set(double nValue);

    private:
        bool _isDouble;
        long lNumber;
        double dNumber;
    };

    class JSONBool : public JSONValue
    {
    public:
        JSONBool(const bool nValue);
        JSONBool(const JSONBool& orig);
        virtual ~JSONBool();

        virtual JSONType type() const override
        {
            return JSONType::BOOL;
        };

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override;

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
        void set(bool nValue);

    private:
        bool value;
    };

    class JSONArray : public JSONValue
    {
    public:
        JSONArray();
        JSONArray(const JSONArray& orig);
        virtual ~JSONArray();

        virtual JSONType type() const override
        {
            return JSONType::ARRAY;
        };

        virtual void format(std::ostream &out, const std::string &current, const std::string &indent) const override;

        /**
         * Get the array of values.
         * @return The array values.
         */
        std::vector<std::shared_ptr<JSONValue>> getArray() const;

        /**
         * Add another value to the array.
         * @param add The value to add.
         */
        void add(const std::shared_ptr<JSONValue> &add);

    private:
        std::vector<std::shared_ptr<JSONValue>> values;
    };

    class JSONDocument
    {
    public:
        JSONDocument();
        /**
         * Create a new JSONDocument from the string.
         * @param doc The string containing the document to parse.
         * @throws ParserError If there was an error parsing the string.
         */
        JSONDocument(const std::string &doc);
        JSONDocument(const JSONDocument& orig);
        virtual ~JSONDocument();

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
        std::string format(const std::string &indent) const;

        /**
         * Prints the document to an output stream without formating.
         * @param out The output stream.
         * @param doc The document to output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream &out, const JSONDocument& doc);

    private:
        std::shared_ptr<JSONValue> root;
    };

}

#endif /* JSONDOCUMENT_H */

