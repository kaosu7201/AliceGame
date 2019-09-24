#pragma once
#include "Object/Object.h"
class Map
  : public Object
{
public:
  Map();
  Map(vector<int> val, int x, int y);
  virtual ~Map();

  void start() {}
  void update() {}
  void Draw() 
  {
    for (int i = 0; i < handle.size(); i++)
    {
      if (handle[i] - 1 != -1)
      {
        SpriteAnimation::Draw("Mapchip", handle[i] - 1, scPos.x, scPos.y);
      }
    }
  }
private:
  vector<int> handle;
  
};

class MapData
{
public:
  static VectorCollision *Collision;

  static void CreateMap(vector<vector<int>> data, int w, int h, int tw, int th);
  static void LoadMapData();
};

bool VectorHitCheck(const D3DXVECTOR2 pos, const D3DXVECTOR2 vec, const VectorCollision collision, D3DXVECTOR2* pOut);