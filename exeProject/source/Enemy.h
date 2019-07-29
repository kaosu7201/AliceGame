#pragma once
#include "appframe.h"
#include "Object/IGettableObject.h"
#include "Collision/CircleCollision.h"
class Enemy :
	public Object , public IGettableObject{
public:
	Enemy();
	virtual ~Enemy();

  void update();
  void start();

	list<shared_ptr<Object>> GetObjects();
private:
	list<shared_ptr<Object>> GettableObject();
  int direction;
  void Draw();
};

