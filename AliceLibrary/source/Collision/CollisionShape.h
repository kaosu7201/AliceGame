#pragma once
#include "../DirectX/DirectX.h"
#include <list>

class CircleCollision;
class Collision2D
{
public:
	Collision2D();
	virtual ~Collision2D();

	D3DXVECTOR3* pos;
	bool getFlag() { return flag; }
	bool getHitFlag() { return HitFlag; }

protected:
	bool flag;
	bool HitFlag;
};

