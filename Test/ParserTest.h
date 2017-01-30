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

#ifndef PARSERTEST_H
#define PARSERTEST_H

#include <iostream>

//-------------------------------------------------------------------------
// struct for XML and JSON tests.

struct Item
{
    std::string Name;
    std::string Location;
    long Qty;
    bool InHouse;
};

//-------------------------------------------------------------------------
// XML test

#include "../Parser/XMLDocument.h"

using namespace KayLib;

std::string xmlString =
        "<?xml version=\"1.0\"?>\
<Inventory>\
  <Date>2015-11-5</Date>\
  <Item InHouse=\"true\" volitile=\"true\">\
    <Name>Toilet Cleaner</Name>\
    <Location>Custodial Department</Location>\
    <Qty>12</Qty>\
  </Item>\
  <Item>\
    <Name>Self removing underwear</Name>\
    <Location>C4-D3T0NAT3</Location>\
    <Qty>327</Qty>\
  </Item>\
  <Item InHouse=\"true\">\
    <Name>Gossip</Name>\
    <Location>Wanda</Location>\
    <Qty>999999</Qty>\
  </Item>\
  <doubleTest>123.456</doubleTest>\
  <exponentTest>2.123456789e+019</exponentTest>\
</Inventory>";

bool testXML()
{
    std::cout << "XML test started..." << std::endl;
    std::cout << "Loading document." << std::endl;
    // Create the document.
    XMLDocument doc(xmlString);
    // Get the root element.
    std::shared_ptr<XMLElement> root = doc.getRoot();
    if(!root)
    {
        std::cout << "Failed to get root element." << std::endl;
        return false;
    }
    std::cout << "Document loaded." << std::endl;
    std::cout << "pretty printed:" << std::endl << doc.format("  ") << std::endl;

    std::string date;
    std::vector<std::shared_ptr < Item>> inventory;

    std::cout << "Reading document." << std::endl;
    std::shared_ptr<XMLElement> inv = root->getFirstChild("Inventory");
    if(inv)
    {
        // Get the date of the inventory.
        std::shared_ptr<XMLElement> dateEle = inv->getFirstChild("Date");
        if(dateEle)
        {
            date = dateEle->getValue();
        }
        else
        {
            std::cout << "Failed to find inventory date." << std::endl;
            return false;
        }
        // Get the items.
        std::vector<std::shared_ptr < XMLElement>> array = inv->getChildren("Item");
        // Iterate the items.
        for(std::shared_ptr<XMLElement> entry : array)
        {
            if(!entry)
            {
                // Got a null pointer, this should not happen!
                std::cout << "XML test: error: null entry" << std::endl;
                return false;
            }
            // Get item information.
            std::shared_ptr<Item> item(new Item());
            for(std::shared_ptr<XMLElement> object : entry->getChildren())
            {
                if(!object)
                {
                    // Got a null pointer, this should not happen!
                    std::cout << "XML test: error: null entry" << std::endl;
                    return false;
                }
                std::string tag = object->getName();
                if(tag == "Name")
                {
                    item->Name = object->getValue();
                }
                else if(tag == "Location")
                {
                    item->Location = object->getValue();
                }
                else if(tag == "InHouse")
                {
                    item->InHouse = object->getValue() == "true";
                }
                else if(tag == "Qty")
                {
                    item->Qty = std::stoi(object->getValue());
                }
            }
            // Add to inventory.
            inventory.push_back(item);
        }
    }
    std::cout << "Showing results." << std::endl;
    // Display results.
    if(!date.empty())
    {
        std::cout << "Found inventory for " << date << std::endl;
    }
    for(auto entry : inventory)
    {
        std::cout << "Found inventory: " << entry->Name << " at " << entry->Location << std::endl;
    }

    std::cout << "XML test complete!" << std::endl;
    std::cout << std::endl;
    return true;
}

//-------------------------------------------------------------------------
// JSON tests

#include "../Parser/JSON.h"
#include "../IO/Exceptions.h"

std::string jsonString =
        "{\
  \"Inventory\": {\
    \"Date\": \"1999-12-31\",\
    \"Item\": [\
      {\
        \"InHouse\": true,\
        \"Name\": \"Toilet Cleaner\",\
        \"Qty\": 12,\
        \"Location\": \"Custodial Department\"\
      },\
      {\
        \"Name\": \"Self removing underwear\",\
        \"Location\": \"C4-D3T0NAT3\",\
        \"Qty\": 327\
      },\
      {\
        \"InHouse\": false,\
        \"Name\": \"Gossip\",\
        \"Qty\": 999999,\
        \"Location\": \"Wanda\"\
      }\
    ],\
    \"doubleTest\" : 123.456,\
    \"exponentTest\" : 2.123456789e+019\
  }\
}";

bool testJSON()
{
    std::cout << "JSON test started..." << std::endl;

    std::shared_ptr<JSONValue> root;
    try
    {
        std::cout << "Loading document." << std::endl;
        // Create the document.
        JSONDocument jDoc(jsonString);
        // Get the root element, this will survive the destruction of the document.
        root = jDoc.getRoot();
        if(!root)
        {
            std::cout << "Failed to get root element." << std::endl;
            return false;
        }
        std::cout << "Document loaded." << std::endl;
        // Print the pretty formated version of the document.
        std::cout << "pretty printed:" << std::endl << jDoc.format("  ") << std::endl;
        //std::cout << "stream format: " << std::endl << jDoc << std::endl;
    }
    catch(ParserException pErr)
    {
        // If there is an error loading the document something has gone wrong.
        std::cout << "JSON error: " << pErr.what() << std::endl;
        return false;
    }
    std::string date;
    std::vector<std::shared_ptr < Item>> inventory;

    std::cout << "Reading document." << std::endl;
    if(!root->isObject())
    {
        std::cout << "Root element is not an ovject." << std::endl;
        return false;
    }
    // Get root as an object.
    std::shared_ptr<JSONObject> obj = std::dynamic_pointer_cast<JSONObject>(root);
    // Find the "Inventory" entry.
    std::shared_ptr<JSONObject> inv = obj->getValueAsObject("Inventory");
    if(inv)
    {
        // Get the date of the inventory.
        date = inv->getString("Date");
        // Get the items.
        std::shared_ptr<JSONArray> array = inv->getValueAsArray("Item");
        if(array)
        {
            // Iterate the items.
            for(std::shared_ptr<JSONValue> entry : array->getArray())
            {
                if(entry.get() == nullptr)
                {
                    // Got a null pointer, this should not happen!
                    std::cout << "JSON test: error: null entry" << std::endl;
                    return false;
                }
                if(entry->isObject())
                {
                    // Get the entry as an object.
                    std::shared_ptr<JSONObject> object = std::dynamic_pointer_cast<JSONObject>(entry);
                    // Get item information.
                    std::shared_ptr<Item> item(new Item());
                    item->Name = object->getString("Name");
                    item->Location = object->getString("Location");
                    item->InHouse = object->getBool("InHouse") == 1;
                    item->Qty = object->getInt("Qty");
                    // Add to inventory.
                    inventory.push_back(item);
                }
            }
        }
    }
    std::cout << "Showing results." << std::endl;
    // Display results.
    if(!date.empty())
    {
        std::cout << "Found inventory for " << date << std::endl;
    }
    for(auto entry : inventory)
    {
        std::cout << "Found inventory: " << entry->Name << " at " << entry->Location << std::endl;
    }

    std::cout << "JSON test complete!" << std::endl;
    std::cout << std::endl;
    return true;
}


#endif /* PARSERTEST_H */
