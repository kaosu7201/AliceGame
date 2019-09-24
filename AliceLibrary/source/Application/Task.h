#pragma once
#include "../Object/ObjectManager.h"
#include "../Texture/DXTexManager.h"
#include "../SpriteAnimation/AnimationManager.h"
#include "../Camera/Camera.h"
class Task
{
public:
	Task();
	virtual ~Task();
	virtual void update();
	virtual void Render();
  void ManagerTerminate();
	static Task *getTask() { return lpTask; }

	ObjectManager* ObjManage;
	DXTexManager* TexManage;
	AnimationManager* AnimManage;
protected:
	static Task *lpTask;
};

