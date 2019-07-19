#include "ObjectManager.h"

list<shared_ptr<ObjectManager>> ObjectManager::ObjectMng;

bool DelList(const shared_ptr<Object> Obj)
{
	if (Obj->GetFlag() == false) {
		return true;
	}
	return false;
}

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{
}


shared_ptr<ObjectManager> ObjectManager::CreateObjectManager(string name, ObjectManager *Manager)
{
	ObjectMng.push_back(shared_ptr<ObjectManager>(Manager));
	Manager->szName = name;
	return ObjectMng.back();
}
