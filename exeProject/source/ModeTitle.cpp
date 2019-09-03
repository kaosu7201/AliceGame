#include "ModeTitle.h"
#include "ModeGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
ModeTitle::ModeTitle()
{
}


ModeTitle::~ModeTitle()
{
}


bool	ModeTitle::Initialize()
{
	if (!base::Initialize()) { return false; }
	return true;
}


bool	ModeTitle::Terminate()
{
	base::Terminate();
	return true;
}


bool	ModeTitle::Process()
{
	_IController *Controller = CDefaultController::GetController();
	base::Process();
	if (Controller->GetButtonState(GCBTN_0,JUST_DOWN))
	{
		// ���̃��[�h���폜�\��
		ModeServer::GetInstance()->Del(this);
		// ���̃��[�h��o�^
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");

    ObjectManager::CreateObjectManager("Map", new ObjectManager());
    MapCreate::LoadMapData();
		ObjectManager::CreateObjectManager("Player", new ObjectManager());
		ObjectManager::addObject(new Player(), "Player");
    ObjectManager::CreateObjectManager("Enemy", new ObjectManager());
    ObjectManager::addObject(new Enemy(), "Enemy");
	}
	return true;
}


bool	ModeTitle::Render()
{	
	DrawTexPack(1, 0, 0, 0);
	DrawTexPack(3, 128, 64, 0);
	DrawTexPack(2, 240, 160, 0);
	base::Render();
	return true;
}