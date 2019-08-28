#include "../appframe.h"

Task* Task::lpTask = NULL;

Task::Task()
{
	lpTask = this;
	ObjManage = ObjectManager::GetManager();
	TexManage = new DXTexManager();
	AnimManage = new AnimationManager();
}


Task::~Task()
{
}


void Task::update()
{
	if (ObjManage) {
		for(auto itr = ObjManage->begin(), end_ = ObjManage->end(); itr != end_; itr++) {
			(*itr)->UpDate();
		}
	}
  ApplicationBase::GetInstance()->GetCamera()->update();
	if (AnimManage) {
		AnimManage->UpDate();
	}
}


void Task::Render()
{
	if (ObjManage) {
		for(auto itr = ObjManage->begin(), end_ = ObjManage->end(); itr != end_; itr++) {
			(*itr)->Draw();
		}
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
  ObjectManager::AllDelManager();
  TexManage->AlldelList();
  AnimManage->AlldelList();
}

