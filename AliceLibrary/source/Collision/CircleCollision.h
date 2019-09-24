#pragma once
#include "../DirectX/DirectX.h"

class CircleCollision {
public:
	CircleCollision(float radius, D3DXVECTOR2 pos);
	virtual ~CircleCollision();

  float posX, posY;
	float radius;
};
