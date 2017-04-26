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

#ifndef XMLDOCUMENT_H
#define XMLDOCUMENT_H

#include <mutex>
#include <vector>
#include <map>
#include <memory>

#include "../IO/Exceptions.h"
#include "StringParser.h"

namespace KayLib
{

    enum class XMLError
    {
        NONE, UnexpectedEndOfDocument, InvalidSyntax, DoubleDashInComment
    };

    std::string XMLErrorString(XMLError err)
    {
        switch(err)
        {
            case XMLError::NONE:
                return "No error";
                break;
            case XMLError::UnexpectedEndOfDocument:
                return "Parser reached the end of string without finishing the document";
                break;
            case XMLError::InvalidSyntax:
                return "Invalid syntax";
                break;
            case XMLError::DoubleDashInComment:
                return "Double dash '--' in comment";
                break;
        }
        return "Unknown error";
    }

    class XMLElement
    {
    public:

        XMLElement(const std::string nme, const std::string val)
        {
            name = nme;
            value = val;
        }

        XMLElement(const XMLElement& orig)
        {
            std::unique_lock<std::mutex> uLock = orig.getLock();
            name = orig.name;
            value = orig.value;
            for(auto attr : orig.attributes)
            {
                attributes[attr.first] = attr.second;
            }
            for(auto child : orig.children)
            {
                std::shared_ptr<XMLElement> nChild(new XMLElement(*child));
                children.push_back(nChild);
            }
        }

        virtual ~XMLElement() { }

        /**
         * Create a copy of the current element.
         */
        std::shared_ptr<XMLElement> copy()
        {
            return std::make_shared<XMLElement>(*this);
        }

        /**
         * Get the name of the element.
         * @return The name of the element.
         */
        std::string getName() const
        {
            return name;
        };

        /**
         * Get the string value of the element.
         * @return The string value of the element.
         */
        std::string getValue() const
        {
            return value;
        };

        /**
         * Get the string value of the element.
         * @return The string value of the element.
         */
        void setValue(std::string val)
        {
            value = val;
        };

        /**
         * Get the names of all attributes of this element.
         * @return The list of attribute names.
         */
        std::vector<std::string> getAttributeNames() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            std::vector<std::string> names;
            for(auto entry : attributes)
            {
                names.push_back(entry.first);
            }
            return names;
        }

        /**
         * Check if the element has the named attribute.
         * @param attr The attribute to check for.
         * @return True if the attribute exists. False if not.
         */
        bool hasAttribute(std::string attr) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return attributes.find(attr) != attributes.end();
        }

        /**
         * Add the attribute with the specified value to the node.
         * @param attr The attribute to add.
         * @param value The value of the attribute.
         */
        void addAttribute(std::string attr, std::string value)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            attributes[attr] = value;
        }

        /**
         * Get the value of the attribute.
         * @param attr The attribute to get.
         * @return The value of the attribute.
         * @throws AttributeNotFound
         */
        std::string getAttribute(std::string attr) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            auto itr = attributes.find(attr);
            if(itr == attributes.end())
            {
                throw AttributeNotFoundException(attr);
            }
            return itr->second;
        }

        /**
         * Add the provided child to the node.
         * @param element The child to add.
         */
        void addChild(std::shared_ptr<XMLElement> element)
        {
            std::unique_lock<std::mutex> uLock = getLock();
            // Add the element to the child list.
            children.push_back(element);
        }

        /**
         * Get all child elements.
         * @return The list of child elements.
         */
        std::vector<std::shared_ptr<XMLElement>> getChildren() const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            return std::vector<std::shared_ptr < XMLElement >> (children);
        }

        /**
         * Does this node have a child with the specified tag name.
         * @param tag The tag to find.
         * @return True if the node has the tag.
         */
        bool hasChild(std::string tag) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            for(auto child : children)
            {
                if(child->name == tag)
                {
                    return true;
                }
            }
            return false;
        }

        /**
         * Get a list of child elements with the specified tag name.
         * @param tag The tag to find.
         * @return The list of elements.
         */
        std::vector<std::shared_ptr<XMLElement>> getChildren(std::string tag) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            std::vector<std::shared_ptr < XMLElement>> childs;
            for(auto child : children)
            {
                if(child->name == tag)
                {
                    childs.push_back(child);
                }
            }
            return childs;
        }

        /**
         * Get the first child element with the specified tag name.
         * @param tag The tag to find.
         * @return The element found.
         */
        std::shared_ptr<XMLElement> getFirstChild(std::string tag) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            for(auto child : children)
            {
                if(child->name == tag)
                {
                    return child;
                }
            }
            return nullptr;
        }

        /**
         * Return a formated string representation of the Value.
         * @param current The current indent.
         * @param indent The indention value to use.  The first line will NOT be indented.
         * @return The formated Value.
         */
        void format(std::ostream &out, const std::string &current, const std::string &indent) const
        {
            std::unique_lock<std::mutex> uLock = getLock();
            std::string nInd = current;
            if(!name.empty())
            {
                nInd += indent;
                out << current << "<" << name;
            }
            for(auto attr : attributes)
            {
                out << " " << attr.first << "=\"" << attr.second << "\"";
            }
            if(value.length() > 0 || !children.empty())
            {
                if(!name.empty() && name != "!--")
                {
                    out << ">";
                }
                if(value.length() > 0)
                {
                    out << value;
                }
                if(!children.empty())
                {
                    if(!name.empty())
                    {
                        out << std::endl;
                    }
                    for(auto child : children)
                    {
                        child->format(out, nInd, indent);
                    }
                    out << current;
                }
                if(!name.empty())
                {
                    if(name == "!--")
                    {
                        out << "-->" << std::endl;
                    }
                    else
                    {
                        out << "</" << name << ">" << std::endl;
                    }
                }
            }
            else
            {
                // Empty tag, no value of children.
                if(!name.empty())
                {
                    out << "/>" << std::endl;
                }
            }
        }

        /**
         * Prints the value to an output stream without formating.
         * @param out The output stream.
         * @param value The value to output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream &out, const XMLElement& value)
        {
            value.format(out, "", "  ");
            return out;
        }

    private:
        mutable std::mutex lockPtr;
        std::string name;
        std::string value;
        std::map<std::string, std::string> attributes;
        std::vector<std::shared_ptr<XMLElement>> children;

        inline std::unique_lock<std::mutex> getLock() const
        {
            return std::unique_lock<std::mutex>(lockPtr);
        }

    };

    class XMLDocument
    {
    public:

        XMLDocument()
        {
            resetError();
        }

        XMLDocument(const std::string &doc)
        {
            resetError();
            parse(doc);
        }

        XMLDocument(const XMLDocument& orig)
        {
            resetError();
            if(root.get() != nullptr)
            {
                root == std::make_shared<XMLElement>(*root);
            }
        }

        virtual ~XMLDocument() { }

        /**
         * Get the documents root element.
         * @return The root element.
         */
        std::shared_ptr<XMLElement> getRoot() const
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
            if(root)
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
        friend std::ostream& operator<<(std::ostream &out, const XMLDocument& doc)
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
        XMLError getError()
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
            lastError = XMLError::NONE;
            errorIndex = -1;
        }

    private:
        std::shared_ptr<XMLElement> root;
        XMLError lastError;
        int errorIndex;

        bool parse(const std::string &doc)
        {
            resetError();
            root = std::make_shared<XMLElement>("", "");
            StringParser<char> parser(doc);
            while(!parser.isEnd())
            {
                parser.skipWhitespace(true);
                if(parser.isEnd())
                {
                    // Apparently there is no root element.
                    break;
                }
                if(parser.peekChar() != '<')
                {
                    // Invalid XML
                    lastError = XMLError::InvalidSyntax;
                    errorIndex = parser.getIndex();
                    return false;
                }
                std::shared_ptr<XMLElement> element = parseElement<char>(parser);
                if(!element)
                {
                    // Error!
                    return false;
                }
                root->addChild(element);
                std::string tag = element->getName();
                if(tag.length() == 0)
                {
                    // Invalid XML.
                    lastError = XMLError::InvalidSyntax;
                    errorIndex = parser.getIndex();
                    return false;
                }
                if(tag == "!--")
                {
                    // We have a comment.
                    continue;
                }
                if(tag == "?xml")
                {
                    // We have a declaration.
                    continue;
                }
                if(tag == "xs:schema")
                {
                    // We have a schema.
                    continue;
                }
                if(tag[0] == '!')
                {
                    // We have a dtd header.
                    continue;
                }
            }

            return true;
        }

        template<typename T>
        std::shared_ptr<XMLElement> parseElement(StringParser<T> &parser)
        {
            // Make sure we are at the next tag.
            parser.skipWhitespace(true);
            std::shared_ptr<XMLElement> element;
            if(parser.nextIs("<?xml", true))
            {
                // Found declaration.
                element = parseDeclaration(parser);
            }
            else if(parser.nextIs("<!--", true))
            {
                // Found comment.
                element = parseComment(parser);
            }
            else if(parser.nextIs("<", true))
            {
                element = parseGeneric(parser);
            }
            return element;
        }

        template<typename T>
        std::shared_ptr<XMLElement> parseComment(StringParser<T> &parser)
        {
            std::shared_ptr<XMLElement> element;
            int start = parser.getIndex();
            int end = start;
            while(!parser.nextIs("--", true))
            {
                T c;
                while((c = parser.peekChar()) != '-' && !parser.isEnd())
                {
                    parser.getChar();
                }
                if(parser.isEnd())
                {
                    lastError = XMLError::UnexpectedEndOfDocument;
                    errorIndex = parser.getIndex();
                    return element;
                }
                //parser.back();
            }
            if(parser.isEnd())
            {
                lastError = XMLError::UnexpectedEndOfDocument;
                errorIndex = parser.getIndex();
                return element;
            }
            if(parser.peekChar() != '>')
            {
                lastError = XMLError::DoubleDashInComment;
                errorIndex = parser.getIndex();
                return element;
            }
            parser.skip(1);
            end = parser.getIndex() - 3;
            std::string comment = parser.getRange(start, (end - start));
            element = std::make_shared<XMLElement>("!--", comment);
            return element;
        }

        template<typename T>
        std::shared_ptr<XMLElement> parseDeclaration(StringParser<T> &parser)
        {
            std::shared_ptr<XMLElement> element = std::make_shared<XMLElement>("?xml", "");
            while(!parser.nextIs("?", true) && !parser.isEnd())
            {
                if(!parseAttribute(parser, element))
                {
                    lastError = XMLError::InvalidSyntax;
                    errorIndex = parser.getIndex();
                    element.reset();
                    return element;
                }
                parser.skipWhitespace(true);
            }
            if(parser.isEnd())
            {
                lastError = XMLError::UnexpectedEndOfDocument;
                errorIndex = parser.getIndex();
                element.reset();
                return element;
            }
            if(parser.peekChar() != '>')
            {
                lastError = XMLError::InvalidSyntax;
                errorIndex = parser.getIndex();
                element.reset();
                return element;
            }
            parser.skip(1);
            return element;
        }

        template<typename T>
        std::basic_string<T> getName(StringParser<T> &parser)
        {
            std::basic_string<T> name;
            while(!parser.isWhitespace(true) && parser.peekChar() != '>' && parser.peekChar() != '=' && !parser.isEnd())
            {
                name += parser.getChar();
            }
            return name;
        }
        
        template<typename T>
        std::shared_ptr<XMLElement> parseGeneric(StringParser<T> &parser)
        {
            std::basic_string<T> tag = getName(parser);
            std::shared_ptr<XMLElement> element = std::make_shared<XMLElement>(tag, "");
            T c = parser.peekChar();
            while((c != '>' && c != '/') && !parser.isEnd())
            {
                if(!parseAttribute(parser, element))
                {
                    lastError = XMLError::InvalidSyntax;
                    errorIndex = parser.getIndex();
                    element.reset();
                    return element;
                }
                parser.skipWhitespace(true);
                c = parser.peekChar();
            }
            if(parser.isEnd())
            {
                lastError = XMLError::UnexpectedEndOfDocument;
                errorIndex = parser.getIndex();
                element.reset();
                return element;
            }
            if(parser.nextIs("/>", true))
            {
                // A complete tag.
                return element;
            }
            if(parser.peekChar() != '>')
            {
                lastError = XMLError::InvalidSyntax;
                errorIndex = parser.getIndex();
                element.reset();
                return element;
            }
            parser.skip(1);
            // An incomplete element.
            parser.skipWhitespace(true);
            std::basic_string<T> endTag = "</" + tag + ">";
            std::basic_string<T> value;
            while(!parser.nextIs(endTag, true))
            {
                if(parser.peekChar() == '<')
                {
                    std::shared_ptr<XMLElement> child = parseElement(parser);
                    element->addChild(child);
                    parser.skipWhitespace(true);
                }
                else
                {
                    value += parser.getTo('<');
                }
            }
            element->setValue(value);
            return element;
        }

        template<typename T>
        bool parseAttribute(StringParser<T> &parser, std::shared_ptr<XMLElement> element)
        {
            parser.skipWhitespace(true);
            std::basic_string<T> attrName = getName(parser);
            if(attrName.length() == 0)
            {
                return false;
            }
            if(parser.peekChar() != '=')
            {
                return false;
            }
            parser.skip(1);
            parser.skipWhitespace(true);
            T c = parser.peekChar();
            if(c != '\'' && c != '"')
            {
                return false;
            }
            std::basic_string<T> attrValue = parser.getQuotedString();
            element->addAttribute(attrName, attrValue);
            parser.skipWhitespace(true);
            return true;
        }
    };

}
#endif /* XMLDOCUMENT_H */

