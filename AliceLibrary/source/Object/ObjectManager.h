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
	static ObjectManager* ObjectMng;
	list<shared_ptr<Object>> ObjectList;
public:
	static ObjectManager* GetManager() { return ObjectMng; }
	ObjectManager();
	virtual ~ObjectManager();

  static void addObject(Object* pObj)
  {
    ObjectMng->ObjectList.push_back(shared_ptr<Object>(pObj));
  }

  void AlldelList()
  {
    ObjectList.clear();
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
      (*itr)->scPosCalc();
			(*itr)->Draw();
		}
	}
};