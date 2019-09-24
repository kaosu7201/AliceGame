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


VectorCollision *MapData::Collision = NULL;


void MapData::CreateMap(vector<vector<int>> data, int w, int h, int tw, int th)
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
    ObjectManager::addObject(new Map(val, (i % w) * tw, (i / w) * th));
  }
}

void MapData::LoadMapData()
{
  Lua lua;
  lua.openfile("res/MapData/map3.lua");
  int w, h, tw, th;
  w = lua["width"].get<int>();
  h = lua["height"].get<int>();
  tw = lua["tilewidth"].get<int>();
  th = lua["tileheight"].get<int>();
  setRange(w, h, tw, th);
  vector<vector<int>> data = ReadData(lua, "tilelayer");
  CreateMap(data, w, h, tw, th);

  vector<D3DXVECTOR2> vec = getVectorArray(lua["layers"][4]["objects"][1]["polyline"].L);
  D3DXVECTOR2 pos = D3DXVECTOR2(lua["layers"][4]["objects"][1]["x"].get<int>(), lua["layers"][4]["objects"][1]["y"].get<int>());
  Collision = new VectorCollision(pos, vec);

  int x, y;
  x = lua["layers"][3]["objects"][1]["x"].get<int>();
  y = lua["layers"][3]["objects"][1]["y"].get<int>();
  ObjectManager::addObject(new Player(x, y));
  lua.close();
}


bool VectorHitCheck(const D3DXVECTOR2 pos, const D3DXVECTOR2 vec, const VectorCollision collision ,D3DXVECTOR2* pOut)
{
  D3DXVECTOR2 nVec1, nVec2, nVec3, nVec4;
  D3DXVECTOR2 P, A, B;
  bool HitFlag = false;
  int i = 0;
  for (i; i < collision.vec.size() - 1; i++)
  {
    P = pos + vec;

    A.x = collision.posX + collision.vec[i].x;
    A.y = collision.posY + collision.vec[i].y;
    B.x = collision.posX + collision.vec[i + 1].x;
    B.y = collision.posY + collision.vec[i + 1].y;

    D3DXVec2Normalize(&nVec1, &(P - A));
    D3DXVec2Normalize(&nVec2, &(B - A));

    D3DXVec2Normalize(&nVec3, &(P - B));
    D3DXVec2Normalize(&nVec4, &(B - A));

    float a = D3DXVec2Dot(&nVec1, &nVec2), b = D3DXVec2Dot(&nVec3, &nVec4);

    if (a > 0 && b < 0)
    {
      D3DXVECTOR2 ap = P - A, ab = B - A;
      float cross = ap.x * ab.y - ab.x * ap.y;

      float result = fabs(cross) / D3DXVec2Length(&ab);
      if (cross > 0)
      {
        pOut->y += result;
      }
      else if (cross < 0)
      {
        pOut->y -= result;
      }
    }
  }
  return HitFlag;
}