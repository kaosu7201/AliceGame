#include "Map.h"
#include "Player.h"
#include "appframe.h"

Map::Map()
{
}

Map::Map(vector<int> val, int x, int y)
{
  position.x = x;
  position.y = y;
  handle = val;
}


Map::~Map()
{
}

void setRange(int width, int height, int tilewidth, int tileheight)
{
ApplicationBase *appbase = ApplicationBase::GetInstance();
  Camera *camera = appbase->GetCamera();
  camera->setRange((width*tilewidth /appbase->DispSizeW())*appbase->DispSizeW(), (height*tileheight / appbase->DispSizeH())*appbase->DispSizeH());
}

vector<D3DXVECTOR2> getVectorArray(lua_State *L)
{
  int fieldindex, arrayindex;
  vector<D3DXVECTOR2> ret;
  int x, y;
  fieldindex = lua_gettop(L);
  lua_pushnil(L);
  int i = 0;
  while (lua_next(L, fieldindex) != 0)
  {
    lua_pushnumber(L, lua_tonumber(L, -2));
    lua_gettable(L, fieldindex);
    arrayindex = lua_gettop(L);
    lua_getfield(L, arrayindex, "x");
    x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, arrayindex, "y");
    y = lua_tonumber(L, -1);
    ret.push_back(D3DXVECTOR2(x, y));
    lua_pop(L, 1);
    lua_pop(L, 2);
    arrayindex = lua_gettop(L);
  }
  lua_settop(L, 1);
  return ret;
}

vector<vector<int>> ReadData(Lua lua, string type)
{
  vector<vector<int>> ret;
  int i = 1;
  while (1)
  {
    if (lua["layers"][i].getPresence() == false) break;
    if (lua["layers"][i]["type"].get<const char*>() == type)
    {
      ret.push_back(lua["layers"][i]["data"].getarray<int>());
    }
    i++;
  }
  return ret;
}


void MapCreate::CreateMap(vector<vector<int>> data, int w, int h, int tw, int th)
{
  for (int i = 0; i < w*h; i++)
  {
    bool f = false;
    vector<int> val;
    for (int j = 0; j < data.size(); j++)
    {
      val.push_back(data[j][i]);
      if (data[j][i] != 0)
      {
        f = true;
      }
    }
    if (f == false) continue;
    ObjectManager::addObject(new Map(val, (i % w) * tw, (i / w) * th), "Map");
  }
}

void MapCreate::LoadMapData()
{
  Lua lua;
  lua.openfile("res/MapData/map2.lua");
  int w, h, tw, th;
  w = lua["width"].get<int>();
  h = lua["height"].get<int>();
  tw = lua["tilewidth"].get<int>();
  th = lua["tileheight"].get<int>();
  setRange(w, h, tw, th);
  vector<vector<int>> data = ReadData(lua, "tilelayer");
  CreateMap(data, w, h, tw, th);
  
  vector<D3DXVECTOR2> vec = getVectorArray(lua["layers"][4]["objects"][1]["polyline"].L);
  int x, y;
  x = lua["layers"][3]["objects"][1]["x"].get<int>();
  y = lua["layers"][3]["objects"][1]["y"].get<int>();
  ObjectManager::addObject(new Player(x, y), "Player");
  lua.close();
}
