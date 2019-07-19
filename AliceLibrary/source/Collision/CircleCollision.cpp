#include "CollisionShape.h"
#include "Collision.h"

CircleCollision::CircleCollision(float radius, D3DXVECTOR3* lppos)
{
	this->radius = radius;
	pos = lppos;
}

CircleCollision::~CircleCollision() {}
