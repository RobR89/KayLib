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

#ifndef LUATEST_H
#define LUATEST_H

#include "Scripting/KLUA.h"
#include "IO/KFile.h"

#include <iostream>
#include <complex>

//-------------------------------------------------------------------------
// LUA tests.

using namespace KayLib;

extern "C"
{
    static int cFunction(lua_State *L)
    {
        double arg = luaL_checknumber(L, 1);
        lua_pushnumber(L, std::sqrt(arg));
        return 1;
    }
}

char LuaProgram[] = "print(\"Hello World!\")\n\
print(\"The value set from C is\", valFromC)\n\
valFromLua = valFromC * 2\n\
\
function luaFunction(what)\n\
  if what == \"Who are you?\" then\
    return \"I am LUA!\"\
  end\
  return \"I don't understand!\"\
end\n\
\
function myFunction(arg)\n\
  return cFunction(arg)\n\
end";

void testLUA()
{
    std::cout << "Starting lUA test." << std::endl;
    std::cout << "Looking for lUA test file." << std::endl;
    std::string tFileName = "Test.lua";
    int recursion = 3;
    while(!KFile::exists(tFileName.c_str()) && recursion > 0)
    {
        tFileName = "../" + tFileName;
        recursion--;
    }
    KLUA *lua = new KLUA();
    if(KFile::exists(tFileName.c_str()))
    {
        std::cout << "Found LUA test file." << std::endl;
        if(!lua->loadFile(tFileName.c_str()))
        {
            std::cout << "Failed to load Test.lua reverting to default code." << std::endl;
            if(!lua->loadProgram(LuaProgram))
            {
                std::cout << "Failed to load program from string." << std::endl;
                return;
            }
        }
    }
    else
    {
        std::cout << "Failed to locate Test.lua reverting to default." << std::endl;
        if(!lua->loadProgram(LuaProgram))
        {
            std::cout << "Failed to load program from string." << std::endl;
            return;
        }
    }

    std::cout << "** Create a global variable in Lua." << std::endl;
    lua->setGlobalNumber(5.7, "valFromC");

    std::cout << "** Execute the program chunk" << std::endl;
    if(!lua->pCall(0, LUA_MULTRET, 0))
    {
        std::cerr << "Something went wrong during execution" << std::endl;
    }

    std::cout << "** Get the variable from Lua." << std::endl;
    double valFromLua = lua->getNumber("valFromLua");
    std::cout << "valFromLua = " << valFromLua << std::endl;

    std::cout << "** Call a LUA function from C++" << std::endl;
    // Get the function to call onto the stack.
    lua->getGlobal("luaFunction");
    // Get the first argument onto the stack.
    lua->pushString("Who are you?");
    lua->pCall(1, 1, 0);
    std::cout << "** The return value of the function is: \"" << lua->getString().c_str() << "\"" << std::endl;

    std::cout << "** Call a C function from LUA" << std::endl;
    // First register the function in LUA.
    lua->setGlobalCFunction(cFunction, "cFunction");
    // Get the function to call onto the stack.
    lua->getGlobal("myFunction");
    // Get the first argument onto the stack.
    lua->pushNumber(4);
    lua->pCall(1, 1, 0);
    std::cout << "** The return value of the function is: " << lua->getNumber() << std::endl;

    std::cout << "Deleting LUA." << std::endl;
    delete lua;
    std::cout << "LUA tests complete." << std::endl;
    std::cout << std::endl;
}


#endif /* LUATEST_H */

