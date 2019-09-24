#pragma once
#include "../DirectX/DirectX.h"

class VectorCollision
{
public:
  VectorCollision();
  VectorCollision(D3DXVECTOR2 pos,vector<D3DXVECTOR2> vec);
  virtual ~VectorCollision();

  vector<D3DXVECTOR2> vec;
  float posX, posY;
};

