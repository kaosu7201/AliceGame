#include "VectorCollision.h"

VectorCollision::VectorCollision()
{
}

VectorCollision::VectorCollision(D3DXVECTOR2 pos, vector<D3DXVECTOR2> vec)
{
  this->posX = pos.x;
  this->posY = pos.y;
  this->vec = vec;
}


VectorCollision::~VectorCollision()
{
}
