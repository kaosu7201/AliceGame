#include "../appframe.h"

Task* Task::lpTask = NULL;

Task::Task()
{
	lpTask = this;
	ObjManage = new ObjectManager();
	TexManage = new DXTexManager();
	AnimManage = new AnimationManager();
}


Task::~Task()
{
}


void Task::update()
{
	if (ObjManage) {
    ObjManage->UpDate();
	}
  ApplicationBase::GetInstance()->GetCamera()->update();
	if (AnimManage) {
		AnimManage->UpDate();
	}
}


void Task::Render()
{
	if (ObjManage) {
    ObjManage->Draw();
	}
	if (TexManage) {
		TexManage->Draw();
	}
	if (AnimManage) {
		AnimManage->Draw();
	}
}

void Task::ManagerTerminate()
{
  ObjManage->AlldelList();
  TexManage->AlldelList();
  AnimManage->AlldelList();
}

