#pragma once
#include "../DirectX/DirectX.h"
#include "AnimationController.h"
#include <list>
class AnimationManager
{
public:
	static AnimationManager* GetManager() { return AnimationMng; }
	AnimationManager();
	virtual ~AnimationManager();
	void addObject(shared_ptr<AnimationController> pObj) { AnimControllList.push_back(pObj); }

	void UpDate()
	{
		for (auto itr = AnimControllList.begin(), end_ = AnimControllList.end(); itr != end_; itr++) {
			(*itr)->update();
		}
	}
	void Draw()
	{
		for (auto itr = AnimControllList.begin(), end_ = AnimControllList.end(); itr != end_; itr++) {
			(*itr)->Draw();
		}
	}
private:
	list<shared_ptr<AnimationController>> AnimControllList;
	static AnimationManager* AnimationMng;
};

