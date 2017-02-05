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
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../IO/Exceptions.h"
#include "StringParser.h"

namespace KayLib
{

    enum class XMLError
    {
        NONE, UnexpectedEndOfDocument, InvalidSyntax
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
                if(!name.empty())
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
                    out << "</" << name << ">" << std::endl;
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
            root = parse(doc);
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

        static std::shared_ptr<XMLElement> parse(const std::string &doc)
        {
            // Put document into a stream for parsing.
            std::stringstream docStream;
            docStream << doc;
            boost::property_tree::ptree tree;
            // Read the XML stream.
            boost::property_tree::xml_parser::read_xml(docStream, tree, boost::property_tree::xml_parser::trim_whitespace);
            return parseNode("", tree);
        }

        static std::shared_ptr<XMLElement> parseNode(const std::string name, const boost::property_tree::ptree &tree)
        {
            std::shared_ptr<XMLElement> node = std::make_shared<XMLElement>(name, tree.get_value<std::string>());
            for(const boost::property_tree::ptree::value_type &entry : tree)
            {
                if(entry.first == "<xmlattr>")
                {
                    // Parse the attribute.
                    for(boost::property_tree::ptree::value_type attr : entry.second)
                    {
                        node->addAttribute(attr.first, attr.second.get_value<std::string>());
                    }
                }
                else
                {
                    // Create new child element.
                    std::shared_ptr<XMLElement> element = parseNode(entry.first, entry.second);
                    // Add the element to the child list.
                    node->addChild(element);
                }
            }
            return node;
        }

    };

}
#endif /* XMLDOCUMENT_H */

