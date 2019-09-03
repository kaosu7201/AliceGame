#include "Map.h"
#include "Lua/Lua.h"
#include "Object/ObjectManager.h"

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

void MapCreate::LoadMapData()
{
  vector<vector<int>> layers;
  Lua lua;
  lua.openfile("res/MapData/map1.lua");
  int w, h, tw, th;
  w = lua["width"].get<int>();
  h = lua["height"].get<int>();
  tw = lua["tilewidth"].get<int>();
  th = lua["tileheight"].get<int>();
  vector<int> a = lua["layers"][1].getarray<int>("data", w*h);
  vector<int> b = lua["layers"][2].getarray<int>("data", w*h);
  for (int i = 0; i < w * h; i++)
  {
    vector<int> val;
    val.push_back(a[i]);
    val.push_back(b[i]);
    ObjectManager::addObject(new Map(val, (i % w) * tw, (i / w) * th), "Map");
  }
  lua.close();
}
