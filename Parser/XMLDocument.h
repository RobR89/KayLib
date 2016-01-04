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

#ifndef XMLDOCUMENT_H
#define	XMLDOCUMENT_H

#include <mutex>
#include <vector>
#include <map>
#include <boost/property_tree/ptree.hpp>

namespace KayLib {

class XMLDocument;

class XMLElement {
  friend XMLDocument;
public:
  XMLElement(const std::string nme, const std::string val);
  XMLElement(const std::string nme, const boost::property_tree::ptree &tree);
  XMLElement(const XMLElement& orig);
  virtual ~XMLElement();

  /**
   * Get the name of the element.
   * @return The name of the element.
   */
  std::string getName() const {
    return name;
  };

  /**
   * Get the string value of the element.
   * @return The string value of the element.
   */
  std::string getValue() const {
    return value;
  };

  /**
   * Get the names of all attributes of this element.
   * @return The list of attribute names.
   */
  std::vector<std::string> getAttributeNames() const;

  /**
   * Check if the element has the named attribute.
   * @param attr The attribute to check for.
   * @return True if the attribute exists. False if not.
   */
  bool hasAttribute(std::string attr) const;

  /**
   * Get the value of the attribute.
   * @param attr The attribute to get.
   * @return The value of the attribute.
   * @throws AttributeNotFound
   */
  std::string getAttribute(std::string attr) const;

  /**
   * Get all child elements.
   * @return The list of child elements.
   */
  std::vector<std::shared_ptr<XMLElement>> getChildren() const;

  /**
   * Get a list of child elements with the specified tag name.
   * @param tag The tag to find.
   * @return The list of elements.
   */
  std::vector<std::shared_ptr<XMLElement>> getChildren(std::string tag) const;

private:
  mutable std::mutex lockPtr;
  std::string name;
  std::string value;
  std::map<std::string, std::string> attributes;
  std::vector<std::shared_ptr<XMLElement>> children;

  inline std::unique_lock<std::mutex> getLock() const {
    return std::unique_lock<std::mutex>(lockPtr);
  }

  std::string format(const std::string current, const std::string indent) const;
};

class XMLDocument {
public:
  XMLDocument();
  XMLDocument(const std::string &doc);
  XMLDocument(std::stringstream &docStream);
  XMLDocument(const XMLDocument& orig);
  virtual ~XMLDocument();

  /**
   * Get the documents root element.
   * @return The root element.
   */
  std::shared_ptr<XMLElement> getRoot() const {
    return root;
  };

  /**
   * Return a formated string representation of the document.
   * @param indent The indention value to use.
   * @return The formated document.
   */
  std::string format(const std::string indent) const;

private:
  std::shared_ptr<XMLElement> root;

};

}
#endif	/* XMLDOCUMENT_H */

