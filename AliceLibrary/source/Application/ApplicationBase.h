
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../DirectX/DirectX.h"
#include "../Mode//ModeServer.h"


class ApplicationBase
{
public:
	ApplicationBase();
	virtual ~ApplicationBase();

	virtual bool Initialize(HINSTANCE hInstance);
	virtual bool Terminate();
	virtual bool Input();
	virtual bool Process();
	virtual bool Render();

	virtual bool AppWindowed() { return true; }
	virtual int DispSizeW() { return 640; }
	virtual int DispSizeH() { return 480; }
	//virtual char* WinTitle() { return (char*)"AliceGame"; }

	static	ApplicationBase	*GetInstance() { return _lpInstance; }
	virtual int GetKey() { return _key; }
	virtual int GetTrg() { return _trg; }

protected:
	static	ApplicationBase	*_lpInstance;

	int		_key, _trg;

private:
	ModeServer *_serverMode;

};
