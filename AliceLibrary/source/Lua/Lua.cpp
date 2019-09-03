#include "Lua.h"



Lua::Lua()
{
}


Lua::~Lua()
{
}

void Lua::openfile(const char* filename)
{
  L = lua_open();
  luaL_openlibs(L);
  luaL_loadfile(L, filename);

  if (lua_pcall(L, 0, -1, 0) != 0)
  {
    lua_close(L);
    exit(EXIT_FAILURE);
  }
}
