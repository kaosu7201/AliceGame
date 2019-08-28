
#include "../appframe.h"
#include "../Camera/Camera.h"
ApplicationBase	*ApplicationBase::_lpInstance = NULL;


ApplicationBase::ApplicationBase() {
	_lpInstance = this;
	DirectDevice = new DXDevice;
}

ApplicationBase::~ApplicationBase() {
}

bool ApplicationBase::Initialize(HINSTANCE hInstance) 
{
	DXDevice *AppDevice = DXDevice::GetDevice();
	if (!AppDevice) { return false; }

	if (!AppDevice->Init(hInstance)) {
		return false;
	}
	Sprite::begin_first();

	srand((unsigned int)time(NULL));

	// モードサーバの初期化
	_serverMode = new ModeServer();

	return true;
}

bool ApplicationBase::Terminate() {
	SAFE_DELETE(_serverMode);
	//SAFE_DELETE(TexManager);


	return true;
}


bool ApplicationBase::Input() {
	_IController *controller = CDefaultController::GetController();
	controller->UpDate();
	return true;
}

bool ApplicationBase::Process() {
	_serverMode->ProcessInit();
	_serverMode->Process();
	_serverMode->ProcessFinish();
	return true;
}

bool ApplicationBase::Render() {
	_serverMode->RenderInit();
	_serverMode->Render();
	_serverMode->RenderFinish();
	return true;
}
