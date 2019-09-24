#include "Collision.h"

bool CircleHitCheck(const CircleCollision &s1, const CircleCollision &s2)
{
	float Distance = s1.radius + s2.radius;
	float sqDistance = Distance * Distance;

	float xc1 = s1.posX, xc2 = s2.posX;
	float yc1 = s1.posY, yc2 = s2.posY;
	float sqX = (xc1 - xc2)*(xc1 - xc2), sqY = (yc1 - yc2)*(yc1 - yc2);
	if(sqX + sqY <= sqDistance) 
	{
		return true;
	}
	return false;
}

// 2Dベクトルの外積
float D3DXVec2Cross(D3DXVECTOR2* v1, D3DXVECTOR2* v2)
{
  return v1->x * v2->y - v1->y * v2->x;
}

