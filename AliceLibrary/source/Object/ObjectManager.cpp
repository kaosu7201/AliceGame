#include "ObjectManager.h"

ObjectManager* ObjectManager::ObjectMng = NULL;

bool DelList(const shared_ptr<Object> Obj)
{
	if (Obj->GetFlag() == false) {
		return true;
	}
	return false;
}

ObjectManager::ObjectManager()
{
  ObjectMng = this;
}

ObjectManager::~ObjectManager()
{
}

