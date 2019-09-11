#pragma once
#include <stdlib.h>
#include "../lua5.1/include/lua.hpp"
#include <vector>
#include "../DirectX/DirectX.h"
using namespace std;
class Lua
{
public:
  Lua();
  virtual ~Lua();
  lua_State *L;
  void openfile(const char* filename);
  void close() { lua_close(L); }

  template<typename T>
  T get() {}
  template<>
  int get<int>()
  {
      int ret = lua_tointeger(L, lua_gettop(L));
      lua_settop(L, 1);
      return ret;
  }
  template<>
  const char* get<const char*>()
  {
    int a = lua_type(L, lua_gettop(L));
    const char* ret = lua_tostring(L, lua_gettop(L));
    lua_settop(L, 1);
    return ret;
  }

  template<typename T1>
  vector<T1> getarray() {}
  template<>
  vector<int> getarray<int>()
  {
    vector<int> ret;
    fieldindex = lua_gettop(L);
    lua_pushnil(L);
    int i = 0;
    while(lua_next(L, fieldindex) != 0)
    {
      ret.push_back(lua_tonumber(L, lua_gettop(L)));
      lua_pop(L, 1);
      i++;
    }
    lua_settop(L, 1);
    return ret;
  }

  bool getPresence()
  {
    bool f = false;
    if (!lua_isnil(L, lua_gettop(L)))
    {
      f = true;
    }
    lua_settop(L, 1);
    return f;
  }

  Lua& operator[](const char* str)
  {
    lua_getfield(L, lua_gettop(L), str);
    fieldindex = lua_gettop(L);
    int a = lua_type(L, lua_gettop(L));
    return *this;
  }

  Lua& operator[](const int i)
  {
    lua_pushnumber(L, i);
    lua_gettable(L, fieldindex);
    fieldindex = lua_gettop(L);
    return *this;
  }

private:
  int fieldindex;
  int arrayindex;
};