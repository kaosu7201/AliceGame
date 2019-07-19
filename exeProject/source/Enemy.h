#pragma once
#include "Object/IGettableObject.h"
#include "Object/Object.h"
class Enemy :
	public Object , public IGettableObject{
public:
	Enemy();
	virtual ~Enemy();

	list<shared_ptr<Object>> GetObjects();
private:
	list<shared_ptr<Object>> GettableObject();
};

