#include "ModeGame.h"
#include "ModeTitle.h"


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
  Task::getTask()->ManagerTerminate();
	return true;
}

bool ModeGame::Process()
{
  _IController *Controller = CDefaultController::GetController();
	base::Process();
  if (Controller->GetButtonState(GCBTN_0, JUST_DOWN))
  {
    // ‚±‚Ìƒ‚[ƒh‚ðíœ—\–ñ
    ModeServer::GetInstance()->Del(this);
    // ŽŸ‚Ìƒ‚[ƒh‚ð“o˜^
    ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");
  }
	return true;
}

bool ModeGame::Render()
{
	base::Render();
	return true;
}
