#include "Collision.h"

bool CircleHitCheck(const CircleCollision &s1, const CircleCollision &s2)
{
	float Distance = s1.getRadius() + s2.getRadius();
	float sqDistance = Distance * Distance;

	float xc1 = s1.pos->x, xc2 = s2.pos->x;
	float yc1 = s1.pos->y, yc2 = s2.pos->y;
	float sqX = (xc1 - xc2)*(xc1 - xc2), sqY = (yc1 - yc2)*(yc1 - yc2);
	if(sqX + sqY <= sqDistance) 
	{
		return true;
	}
	return false;
}