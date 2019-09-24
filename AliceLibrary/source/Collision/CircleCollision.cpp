#include "CircleCollision.h"

CircleCollision::CircleCollision(float radius, D3DXVECTOR2 pos)
{
	this->radius = radius;
  posX = pos.x;
  posY = pos.y;
}

CircleCollision::~CircleCollision() {}
