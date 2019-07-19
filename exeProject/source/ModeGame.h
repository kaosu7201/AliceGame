#pragma once
#include "appframe.h"
class ModeGame :
	public ModeBase
{
	typedef ModeBase base;
public:
	ModeGame();
	virtual ~ModeGame();

	bool	Initialize();
	bool	Terminate();
	bool	Process();
	bool	Render();
};


