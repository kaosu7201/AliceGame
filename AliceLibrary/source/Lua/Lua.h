#pragma once
#include <stdlib.h>
#include "../lua5.1/include/lua.hpp"
#include <vector>
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
  T get()
  {
    T ret;
    ret = lua_tointeger(L, lua_gettop(L));
    lua_settop(L, 1);
    return ret;
  }
  template<typename T1>
  vector<T1> getarray(const char *s, int num)
  {
    vector<T1> ret(num);
    lua_getfield(L, fieldindex, s);
    fieldindex = lua_gettop(L);
    for (int i = 0; i < num; i++)
    {
      lua_pushnumber(L, i + 1);
      lua_gettable(L, fieldindex);
      if (lua_type(L, lua_gettop(L)) == 0)
      {
        lua_settop(L, 1);
      }
      ret[i] = lua_tonumber(L, lua_gettop(L));
      lua_pop(L, 1);
    }
    lua_settop(L, 1);
    return ret;
  }

  Lua& operator[](const char* str)
  {
    lua_getfield(L, lua_gettop(L), str);
    fieldindex = lua_gettop(L);
    return *this;
  }

  Lua& operator[](const int i)
  {
    lua_pushnumber(L, i);
    lua_gettable(L, fieldindex);
    fieldindex = lua_gettop(L);
    if (lua_type(L, fieldindex) == 0)
    {
      lua_settop(L, 1);
    }
    return *this;
  }

private:
  int fieldindex;
  int arrayindex;
};