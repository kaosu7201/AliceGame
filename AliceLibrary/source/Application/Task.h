#pragma once
#include "../Object/ObjectManager.h"
#include "../Texture/DXTexManager.h"
#include "../SpriteAnimation/AnimationManager.h"
class Task
{
public:
	Task();
	virtual ~Task();
	virtual void update();
	virtual void Render();
  void ManagerTerminate();
	static Task *getTask() { return lpTask; }

	list<shared_ptr<ObjectManager>>* ObjManage;
	DXTexManager* TexManage;
	AnimationManager* AnimManage;
protected:
	static Task *lpTask;
};

