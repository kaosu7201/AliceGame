#pragma once
#include "../DirectX/DirectX.h"
#include "Object.h"
#include <list>
#include <algorithm>
using namespace std;

bool DelList(const shared_ptr<Object> Obj);

class ObjectManager
{
private:
	string szName;
	static list<shared_ptr<ObjectManager>> ObjectMng;
	list<shared_ptr<Object>> ObjectList;
public:
	static list<shared_ptr<ObjectManager>>* GetManager() { return &ObjectMng; }
	ObjectManager();
	virtual ~ObjectManager();

	static void addObject(Object* pObj, string name) 
	{  
		for(auto itr = ObjectMng.begin(), end_ = ObjectMng.end(); itr != end_; itr++)
		{
			if((*itr)->szName == name)
			{
				(*itr)->ObjectList.push_back(shared_ptr<Object>(pObj));
			}
		}
	}

	static shared_ptr<ObjectManager> CreateObjectManager(string name, ObjectManager *Manager);
  static void delObjectManager(string name)
  {
    for (auto itr = ObjectMng.begin(); itr != ObjectMng.end();)
    {
      if ((*itr)->szName == name)
      {
        ObjectMng.erase(itr);
        break;
      }
      itr++;
    }
  }
	void UpDate() 
	{
		for (auto itr = ObjectList.begin(), end_ = ObjectList.end(); itr != end_; itr++) {
			if ((*itr)->GetStartFlag() == false)
			{
				(*itr)->start();
				(*itr)->SetStartFlag(true);
			}
			(*itr)->update();
		}
		ObjectList.remove_if(DelList);
	}
	void Draw()
	{
		for (auto itr = ObjectList.begin(), end_ = ObjectList.end(); itr != end_; itr++) {
			(*itr)->Draw();
		}
	}
};