#include "ModeGame.h"



ModeGame::ModeGame()
{
}


ModeGame::~ModeGame()
{
}

bool ModeGame::Initialize()
{
	if (!base::Initialize()) { return false; }
	return true;
}

bool ModeGame::Terminate()
{
	base::Terminate();
	return true;
}

bool ModeGame::Process()
{
	base::Process();
	return true;
}

bool ModeGame::Render()
{
	base::Render();
	return true;
}
