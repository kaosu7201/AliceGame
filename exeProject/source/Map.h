#pragma once
#include "Object/Object.h"
class Map
  : public Object
{
public:
  Map();
  virtual ~Map();

};

class MapCreate
{
public:
  static void SaveMapData();
  static void LoadMapData();
};