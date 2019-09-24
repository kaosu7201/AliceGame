#include "RectangleCollision.h"

RectangleCollision::RectangleCollision(float top, float down, float left, float right, string *ID)
{
	this->top = top;
	this->down = down;
	this->left = left;
	this->right = right;
}


RectangleCollision::RectangleCollision(RECT rect, string *ID)
{
	top = rect.top;
	down = rect.bottom;
	left = rect.left;
	right = rect.right;
}


RectangleCollision::~RectangleCollision() 
{

}

void RectangleCollision::update() {}
