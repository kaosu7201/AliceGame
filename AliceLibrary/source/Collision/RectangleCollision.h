#pragma once

#include "../DirectX/DirectX.h"
class RectangleCollision {
public:
	RectangleCollision(float top, float down, float left, float right, string *ID);
	RectangleCollision(RECT rect, string *ID);
	virtual ~RectangleCollision();

	void update();

	float top;
	float down;
	float left;
	float right;
  float posX, posY;
};