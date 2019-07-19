#pragma once
#include "CollisionShape.h"

class Collision2D;
class RectangleCollision : public Collision2D {
public:
	RectangleCollision(float top, float down, float left, float right, string *ID);
	RectangleCollision(RECT rect, string *ID);
	virtual ~RectangleCollision();

	void update();
private:
	float top;
	float down;
	float left;
	float right;
};