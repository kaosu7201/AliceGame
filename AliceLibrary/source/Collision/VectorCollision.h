#pragma once
#include "CollisionShape.h"
class VectorCollision :
  public Collision2D
{
public:
  VectorCollision();
  VectorCollision(D3DXVECTOR3 pos,vector<D3DXVECTOR2> vec);
  virtual ~VectorCollision();
private:
  vector<D3DXVECTOR2> vec;
};

