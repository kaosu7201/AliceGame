#pragma once
#include "CollisionShape.h"

class CircleCollision : public Collision2D {
public:
	CircleCollision(float radius, D3DXVECTOR3* lppos);
	virtual ~CircleCollision();
	float getRadius() const { return radius; }
private:
	float radius;
};
