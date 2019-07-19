#pragma once
#include "Object.h"
#include <list>
using namespace std;
class IGettableObject {
public:
	IGettableObject();
	virtual ~IGettableObject();
	virtual list<shared_ptr<Object>> GettableObject() = 0;
};

