#pragma once
#include "../appframe.h"

class UserInterface
{
public:
	UserInterface();
	virtual ~UserInterface();
	virtual void Draw() = 0;
protected:
	float posX, posY;
};

class UICampus :
	public Object
{
public:
	UICampus();
	virtual ~UICampus();
	
	void Setpivot(float x,float y)
	{
		pivotX = x;
		pivotY = y;
	}
	void Setpivot(D3DXVECTOR2 vec)
	{
		pivotX = vec.x;
		pivotY = vec.y;
	}
	void SetCampusRect(RECT rect)
	{
		CampusRect = rect;
	}
	void Start();
	void UpDate();

	void AddUI(UserInterface* lpUI)
	{
		UIList.push_back(lpUI);
	}
protected:
	list<UserInterface*> UIList;
	float pivotX, pivotY;
	RECT CampusRect;

	void Draw();
};
