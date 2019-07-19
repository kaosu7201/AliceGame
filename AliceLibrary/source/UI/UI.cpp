#include "UI.h"


UserInterface::UserInterface()
{
}


UserInterface::~UserInterface()
{
}


UICampus::UICampus()
{
}


UICampus::~UICampus()
{
}


void UICampus::Start()
{
	ApplicationBase *appBase = ApplicationBase::GetInstance();
	Setpivot(0, 0);
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = appBase->DispSizeW();
	rect.bottom = appBase->DispSizeH();
	SetCampusRect(rect);
}


void UICampus::UpDate()
{
	Draw();
}

void UICampus::Draw()
{	
	for (auto itr = UIList.begin(), end_ = UIList.end(); itr != end_; itr++) {
		(*itr)->Draw();
	}
}
