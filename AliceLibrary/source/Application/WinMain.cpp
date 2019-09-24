/*
** WinMain
*/

//
// include 部
//

#include "../appframe.h"

// DirectX用旧関数エイリアス定義
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
int (WINAPIV * _sscanf)(const char*, const char*, ...) = sscanf;
int (WINAPIV * __snprintf)(char *, size_t, const char*, ...) = _snprintf;
int (WINAPIV * _sprintf)(char*, const char*, ...) = sprintf;

#define _CRT_SECURE_NO_DEPRECATE								// 旧形式の関数使用

//
// WinMain(). プログラム起動関数
//
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
) {
	ApplicationBase *appBase = ApplicationBase::GetInstance();
	Fps fps;
	if (!appBase) { return 0; }
	Task task;
	if (!appBase->Initialize(hInstance)) {
		return 0;
	}
	IController controller = CDefaultController::Create(CDIKeyBoard::Create(),CDIMouse::Create());
	controller->SetDefault();
  controller->UpDate();
	while (ProcessEvent()) {
		fps.Update();
		appBase->Input();
		appBase->Process();

		HCls();
		appBase->Render();
		fps.Draw();
		Flip();
		fps.Wait();
	}

	appBase->Terminate();
	return 0;
}
