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

class MapCreate
{
public:
  static void CreateMap(vector<vector<int>> data, int w, int h, int tw, int th);
  static void LoadMapData();
};