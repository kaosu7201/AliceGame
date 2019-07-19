#pragma once
#include "appframe.h"
class ModeTitle :
	public ModeBase
{
	typedef ModeBase base;
public:
	ModeTitle();
	virtual ~ModeTitle();

	bool	Initialize();
	bool	Terminate();
	bool	Process();
	bool	Render();
};

