#pragma once
#include "appframe.h"
#include "Object/IGettableObject.h"
#include "Collision/CircleCollision.h"

class Player :
	public Object, public IGettableObject
{
public:
	Player();
	virtual ~Player();

	void start();
	void update();

private:
	list<shared_ptr<Object>> GettableObject();
	_IController* controller;
  int direction;
	void Draw();
};

