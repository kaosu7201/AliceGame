#include "VectorCollision.h"



VectorCollision::VectorCollision()
{
}

VectorCollision::VectorCollision(D3DXVECTOR3 pos, vector<D3DXVECTOR2> vec)
{
  this->pos = pos;
  this->vec = vec;
}


VectorCollision::~VectorCollision()
{
}
