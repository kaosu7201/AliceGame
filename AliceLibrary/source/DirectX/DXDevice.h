#pragma once
// DirectX9ÇÃÉwÉbÉ_Å[Çä‹ÇﬂÇÈ
#include "DXInput.h"

class DXDevice
{
public:
	DXDevice();
	~DXDevice();

	IDirect3D9*		pD3D;
	IDirect3DDevice9*	pDevice;
	HWND hWnd;
	bool Init(HINSTANCE hInstance);
	bool WindowInit(HINSTANCE hInstance);
	bool DirectInit();

	static DXDevice* GetDevice() {
		return lpDevice;
	}
	HINSTANCE GetHInstance() { return hInst; }
private:
	static DXDevice* lpDevice;
	DXInput* lpDXInput;
	HINSTANCE hInst;
};

extern DXDevice *DirectDevice;