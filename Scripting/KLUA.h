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

#ifndef KLUA_H
#define KLUA_H

extern "C"
{
#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <lua5.2/lauxlib.h>
}
#include <string>

namespace KayLib
{

    class KLUA
    {
    public:
        KLUA();
        KLUA(const KLUA& orig) = delete;
        virtual ~KLUA();

        /**
         * Load program from file.
         * @param file The file to load.
         * @return True if successful.
         */
        bool loadFile(std::string file);
        /**
         * Load program from string
         * @param prog The string contining the program.
         * @return True if successful.
         */
        bool loadProgram(std::string prog);

        /**
         * Get the last string representation of the last error.
         * @return The error string.
         */
        std::string getError();
        /**
         * Get the last string representation of the last LUA error.
         * @return The error string.
         */
        std::string luaError();

        /**
         * Call a LUA function.
         * @param nArgs The number of arguments passed to the function.
         * @param nResults The number of results returned from the function.
         * @return True if successful.
         * @note the function call must be pushed on the stack first, followed by the parameters.
         */
        bool call(int nArgs, int nResults);
        /**
         * Protected call of a LUA function.
         * @param nArgs The number of arguments passed to the function.
         * @param nResults The number of results returned from the function.
         * @param errFunct The index of the error function.
         * @return True if successful.
         * @note the function call must be pushed on the stack first, followed by the parameters.
         */
        bool pCall(int nArgs, int nResults, int errFunct);

        /**
         * Pushes global value on top of the stack.
         * @name name The name of the global.
         * @return True if successful.
         */
        bool getGlobal(std::string name);
        /**
         * Removes the top value from the stack and sets it as a global.
         * @name name The name of the global.
         * @return True if successful.
         */
        bool setGlobal(std::string name);

        /**
         * Push a cFunction onto the stack.
         * @param funct The function to push on the stack.
         * @return True if successful.
         */
        bool pushCFunction(lua_CFunction funct);
        /**
         * Push a cFunction through the stack to a global value.
         * @param funct The function to assign.
         * @param name The name of the global.
         * @return True if successful.
         * @note Same as PushCFunction();SetGlobal();
         */
        bool setGlobalCFunction(lua_CFunction funct, std::string name);
        /**
         * Push a number onto the stack.
         * @param number The number to push on the stack.
         * @return True if successful.
         */
        bool pushNumber(lua_Number number);
        /**
         * Push a number through the stack to a global value.
         * @param number The number to assign.
         * @param name The name of the global.
         * @return True if successful.
         * @note Same as PushNumber();SetGlobal();
         */
        bool setGlobalNumber(lua_Number number, std::string name);
        /**
         * Push a string onto the stack.
         * @param str The string to push on the stack.
         * @return True if successful.
         */
        bool pushString(std::string str);
        /**
         * Push a string through the stack to a global value.
         * @param str The string to assign.
         * @param name The name of the global.
         * @return True if successful.
         * @note Same as PushString();SetGlobal();
         */
        bool setGlobalString(std::string str, std::string name);

        /**
         * Pops the top value off the stack and returns it as a string.
         * @return The string from the top of the stack.
         */
        std::string getString();
        /**
         * Pulls a global value through the stack and returns it as a string.
         * @param name The name of the global.
         * @return The string.
         * @note Same as GetGlobal();GetString();
         */
        std::string getString(std::string name);
        /**
         * Pops the top value off the stack and returns it as a number.
         * @return The number from the top of the stack.
         */
        lua_Number getNumber();
        /**
         * Pulls a global value through the stack and returns it as a number.
         * @param name The name of the global.
         * @return The number.
         * @note Same as GetGlobal();GetNumber();
         */
        lua_Number getNumber(std::string name);

    protected:
        lua_State *state;
        std::string lastError;

    };

}

#endif /* KLUA_H */

