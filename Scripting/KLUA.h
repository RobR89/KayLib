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
    protected:
        lua_State *state;
        std::string lastError;

    public:

        KLUA()
        {
            state = luaL_newstate();
            luaL_openlibs(state);
        }
        
        KLUA(const KLUA& orig) = delete;

        virtual ~KLUA()
        {
            lua_close(state);
        }

        /**
         * Load program from file.
         * @param file The file to load.
         * @return True if successful.
         */
        bool loadFile(std::string file)
        {
            if(state == nullptr)
            {
                return false;
            }
            int res = luaL_loadfile(state, file.c_str());
            if(res == LUA_ERRSYNTAX)
            {
                lastError = "LoadFile: Syntax error: " + luaError();
                return false;
            }
            if(res == LUA_ERRMEM)
            {
                lastError = "LoadFile: Memory error: " + luaError();
                return false;
            }
            if(res != 0)
            {
                lastError = "LoadFile: Unknown error: " + luaError();
                return false;
            }
            return true;
        }

        /**
         * Load program from string
         * @param prog The string contining the program.
         * @return True if successful.
         */
        bool loadProgram(std::string prog)
        {
            if(state == nullptr)
            {
                return false;
            }
            int res = luaL_loadstring(state, prog.c_str());
            if(res == LUA_ERRSYNTAX)
            {
                lastError = "LoadProgram: Syntax error: " + luaError();
                return false;
            }
            if(res == LUA_ERRMEM)
            {
                lastError = "LoadProgram: Memory error: " + luaError();
                return false;
            }
            if(res != 0)
            {
                lastError = "LoadProgram: Unknown error: " + luaError();
                return false;
            }
            return true;
        }

        /**
         * Get the last string representation of the last error.
         * @return The error string.
         */
        std::string getError()
        {
            if(state == nullptr)
            {
                return "No State.";
            }
            return lastError;
        }

        /**
         * Get the last string representation of the last LUA error.
         * @return The error string.
         */
        std::string luaError()
        {
            if(state == nullptr)
            {
                return "No State.";
            }
            std::string err = lua_tostring(state, -1);
            lua_pop(state, 1);
            return err;
        }

        /**
         * Call a LUA function.
         * @param nArgs The number of arguments passed to the function.
         * @param nResults The number of results returned from the function.
         * @return True if successful.
         * @note the function call must be pushed on the stack first, followed by the parameters.
         */
        bool call(int nArgs, int nResults)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_call(state, nArgs, nResults);
            return true;
        }

        /**
         * Protected call of a LUA function.
         * @param nArgs The number of arguments passed to the function.
         * @param nResults The number of results returned from the function.
         * @param errFunct The index of the error function.
         * @return True if successful.
         * @note the function call must be pushed on the stack first, followed by the parameters.
         */
        bool pCall(int nArgs, int nResults, int errFunct)
        {
            if(state == nullptr)
            {
                return false;
            }
            int res = lua_pcall(state, nArgs, nResults, errFunct);
            if(res == LUA_ERRRUN)
            {
                lastError = "PCall: Runtime error: " + luaError();
                return false;
            }
            if(res == LUA_ERRMEM)
            {
                lastError = "PCall: Memory error: " + luaError();
                return false;
            }
            if(res == LUA_ERRERR)
            {
                lastError = "PCall: Error error: " + luaError();
                return false;
            }
            if(res != 0)
            {
                lastError = "PCall: Unknown error: " + luaError();
                return false;
            }
            return true;
        }

        /**
         * Pushes global value on top of the stack.
         * @name name The name of the global.
         * @return True if successful.
         */
        bool getGlobal(std::string name)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_getglobal(state, name.c_str());
            return true;
        }

        /**
         * Removes the top value from the stack and sets it as a global.
         * @name name The name of the global.
         * @return True if successful.
         */
        bool setGlobal(std::string name)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_setglobal(state, name.c_str());
            return true;
        }

        /**
         * Push a cFunction onto the stack.
         * @param funct The function to push on the stack.
         * @return True if successful.
         */
        bool pushCFunction(lua_CFunction funct)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_pushcfunction(state, funct);
            return true;
        }

        /**
         * Push a cFunction through the stack to a global value.
         * @param funct The function to assign.
         * @param name The name of the global.
         * @return True if successful.
         * @note Same as PushCFunction();SetGlobal();
         */
        bool setGlobalCFunction(lua_CFunction funct, std::string name)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_pushcfunction(state, funct);
            lua_setglobal(state, name.c_str());
            return true;
        }

        /**
         * Push a number onto the stack.
         * @param number The number to push on the stack.
         * @return True if successful.
         */
        bool pushNumber(lua_Number number)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_pushnumber(state, number);
            return true;
        }

        /**
         * Push a number through the stack to a global value.
         * @param number The number to assign.
         * @param name The name of the global.
         * @return True if successful.
         * @note Same as PushNumber();SetGlobal();
         */
        bool setGlobalNumber(lua_Number number, std::string name)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_pushnumber(state, number);
            lua_setglobal(state, name.c_str());
            return true;
        }

        /**
         * Push a string onto the stack.
         * @param str The string to push on the stack.
         * @return True if successful.
         */
        bool pushString(std::string str)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_pushstring(state, str.c_str());
            return true;
        }

        /**
         * Push a string through the stack to a global value.
         * @param str The string to assign.
         * @param name The name of the global.
         * @return True if successful.
         * @note Same as PushString();SetGlobal();
         */
        bool setGlobalString(std::string str, std::string name)
        {
            if(state == nullptr)
            {
                return false;
            }
            lua_pushstring(state, str.c_str());
            lua_setglobal(state, name.c_str());
            return true;
        }

        /**
         * Pops the top value off the stack and returns it as a string.
         * @return The string from the top of the stack.
         */
        std::string getString()
        {
            if(state == nullptr)
            {
                return "";
            }
            std::string str = lua_tostring(state, -1);
            lua_pop(state, 1);
            return str;
        }

        /**
         * Pulls a global value through the stack and returns it as a string.
         * @param name The name of the global.
         * @return The string.
         * @note Same as GetGlobal();GetString();
         */
        std::string getString(std::string name)
        {
            if(state == nullptr)
            {
                return "";
            }
            lua_getglobal(state, name.c_str());
            std::string str = lua_tostring(state, -1);
            lua_pop(state, 1);
            return str;
        }

        /**
         * Pops the top value off the stack and returns it as a number.
         * @return The number from the top of the stack.
         */
        lua_Number getNumber()
        {
            if(state == nullptr)
            {
                return 0;
            }
            lua_Number num = lua_tonumber(state, -1);
            lua_pop(state, 1);
            return num;
        }

        /**
         * Pulls a global value through the stack and returns it as a number.
         * @param name The name of the global.
         * @return The number.
         * @note Same as GetGlobal();GetNumber();
         */
        lua_Number getNumber(std::string name)
        {
            if(state == nullptr)
            {
                return 0;
            }
            lua_getglobal(state, name.c_str());
            lua_Number num = lua_tonumber(state, -1);
            lua_pop(state, 1);
            return num;
        }

    };

}

#endif /* KLUA_H */

