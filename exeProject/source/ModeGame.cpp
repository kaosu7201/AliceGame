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
	return true;
}

bool ModeGame::Process()
{
  _IController *Controller = CDefaultController::GetController();
	base::Process();
  if (Controller->GetButtonState(GCBTN_0, JUST_DOWN))
  {
    // ���̃��[�h���폜�\��
    ModeServer::GetInstance()->Del(this);
    // ���̃��[�h��o�^
    ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");

    ObjectManager::delObjectManager("Player");
  }
	return true;
}

bool ModeGame::Render()
{
	base::Render();
	return true;
}
