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
  ApplicationBase *appbase = ApplicationBase::GetInstance();
  Camera *camera = appbase->GetCamera();
  camera->setRange((w*tw/appbase->DispSizeW())*appbase->DispSizeW(), (h*th / appbase->DispSizeH())*appbase->DispSizeH());

  vector<int> a = lua["layers"][1].getarray<int>("data", w*h);
  vector<int> b = lua["layers"][2].getarray<int>("data", w*h);
  for (int i = 0; i < w * h; i++)
  {
    if (a[i] == 0 && b[i] == 0) continue;
    vector<int> val;
    val.push_back(a[i]);
    val.push_back(b[i]);
    ObjectManager::addObject(new Map(val, (i % w) * tw, (i / w) * th), "Map");
  }

  int x, y;
  x = lua["layers"][3]["objects"][1]["x"].get<int>();
  y = lua["layers"][3]["objects"][1]["y"].get<int>();
  ObjectManager::addObject(new Player(x, y), "Player");
  lua.close();
}
