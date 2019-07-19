
#include "appframe.h"
#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "SpriteAnimation/SpriteAnimation.h"
// ����
ApplicationMain				g_oApplicationMain;
bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }
	DXTexManager::GetManagerInstance()->CreateTexturePack("res/TexPack/");
	SpriteAnimation::addAnimePack("PlayerIdle", "res/Texture/Player/Idle.png", 64, 64, 4);
	SpriteAnimation::addAnimePack("EnemyIdle", "res/Texture/Enemy/Idle.png", 64, 64, 1);
	// ���[�h�̓o�^
	ModeServer::GetInstance()->Add(new ModeTitle(), 1, "title");

	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}


bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

