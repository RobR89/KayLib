#include "KLUA.h"

namespace KayLib {

KLUA::KLUA() {
  state = luaL_newstate();
  luaL_openlibs(state);
}

KLUA::~KLUA() {
  lua_close(state);
}

bool KLUA::loadFile(std::string file) {
  if(state == nullptr) {
    return false;
  }
  int res = luaL_loadfile(state, file.c_str());
  if(res == LUA_ERRSYNTAX) {
    lastError = "LoadFile: Syntax error: " + luaError();
    return false;
  }
  if(res == LUA_ERRMEM) {
    lastError = "LoadFile: Memory error: " + luaError();
    return false;
  }
  if(res != 0) {
    lastError = "LoadFile: Unknown error: " + luaError();
    return false;
  }
  return true;
}

bool KLUA::loadProgram(std::string prog) {
  if(state == nullptr) {
    return false;
  }
  int res = luaL_loadstring(state, prog.c_str());
  if(res == LUA_ERRSYNTAX) {
    lastError = "LoadProgram: Syntax error: " + luaError();
    return false;
  }
  if(res == LUA_ERRMEM) {
    lastError = "LoadProgram: Memory error: " + luaError();
    return false;
  }
  if(res != 0) {
    lastError = "LoadProgram: Unknown error: " + luaError();
    return false;
  }
  return true;
}

std::string KLUA::getError() {
  if(state == nullptr) {
    return "No State.";
  }
  return lastError;
}

std::string KLUA::luaError() {
  if(state == nullptr) {
    return "No State.";
  }
  std::string err = lua_tostring(state, -1);
  lua_pop(state, 1);
  return err;
}

bool KLUA::call(int nArgs, int nResults) {
  if(state == nullptr) {
    return false;
  }
  lua_call(state, nArgs, nResults);
  return true;
}

bool KLUA::pCall(int nArgs, int nResults, int errFunct) {
  if(state == nullptr) {
    return false;
  }
  int res = lua_pcall(state, nArgs, nResults, errFunct);
  if(res == LUA_ERRRUN) {
    lastError = "PCall: Runtime error: " + luaError();
    return false;
  }
  if(res == LUA_ERRMEM) {
    lastError = "PCall: Memory error: " + luaError();
    return false;
  }
  if(res == LUA_ERRERR) {
    lastError = "PCall: Error error: " + luaError();
    return false;
  }
  if(res != 0) {
    lastError = "PCall: Unknown error: " + luaError();
    return false;
  }
  return true;
}

bool KLUA::getGlobal(std::string name) {
  if(state == nullptr) {
    return false;
  }
  lua_getglobal(state, name.c_str());
  return true;
}

bool KLUA::setGlobal(std::string name) {
  if(state == nullptr) {
    return false;
  }
  lua_setglobal(state, name.c_str());
  return true;
}

bool KLUA::pushCFunction(lua_CFunction funct) {
  if(state == nullptr) {
    return false;
  }
  lua_pushcfunction(state, funct);
  return true;
}

bool KLUA::setGlobalCFunction(lua_CFunction funct, std::string name) {
  if(state == nullptr) {
    return false;
  }
  lua_pushcfunction(state, funct);
  lua_setglobal(state, name.c_str());
  return true;
}

bool KLUA::pushNumber(lua_Number number) {
  if(state == nullptr) {
    return false;
  }
  lua_pushnumber(state, number);
  return true;
}

bool KLUA::setGlobalNumber(lua_Number number, std::string Name) {
  if(state == nullptr) {
    return false;
  }
  lua_pushnumber(state, number);
  lua_setglobal(state, Name.c_str());
  return true;
}

bool KLUA::pushString(std::string str) {
  if(state == nullptr) {
    return false;
  }
  lua_pushstring(state, str.c_str());
  return true;
}

bool KLUA::setGlobalString(std::string str, std::string Name) {
  if(state == nullptr) {
    return false;
  }
  lua_pushstring(state, str.c_str());
  lua_setglobal(state, Name.c_str());
  return true;
}

std::string KLUA::getString() {
  if(state == nullptr) {
    return "";
  }
  std::string str = lua_tostring(state, -1);
  lua_pop(state, 1);
  return str;
}

std::string KLUA::getString(std::string name) {
  if(state == nullptr) {
    return "";
  }
  lua_getglobal(state, name.c_str());
  std::string str = lua_tostring(state, -1);
  lua_pop(state, 1);
  return str;
}

lua_Number KLUA::getNumber() {
  if(state == nullptr) {
    return 0;
  }
  lua_Number num = lua_tonumber(state, -1);
  lua_pop(state, 1);
  return num;
}

lua_Number KLUA::getNumber(std::string name) {
  if(state == nullptr) {
    return 0;
  }
  lua_getglobal(state, name.c_str());
  lua_Number num = lua_tonumber(state, -1);
  lua_pop(state, 1);
  return num;
}

}
