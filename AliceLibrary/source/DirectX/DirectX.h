#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

#include "../Application/UtilMacro.h"
#include "../TexturePack/libtexpack.h"

#include "DXDevice.h"
#include "DXInput.h"
#include "DXShader.h"
// DirectX9の静的ライブラリをリンク
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment( lib, "winmm")

extern int GetNowTime(int nDummy);

extern int GetPresentTime(void);

LRESULT CALLBACK    WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define MAX_EVENTS 10
// プロセス制御変数
extern BOOL gg_bEndFlag;
int ProcessEvent();

int HCls(void);
int Flip(void);
bool TextureLoad(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *pTexture);
bool TextureLoad(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *pTexture, int Width, int Height);

HRESULT InitStringRender();
void DrawPrintf(int x, int y, int nColor, const char *fmt, ...);

// define
#define DINPUT_KEYBOARD		(1<<0)
#define DINPUT_MOUSE			(1<<1)
#define DINPUT_JOYSTICK		(1<<2)

int GetColor(int r, int g, int b);
int GetColor(int a, int r, int g, int b);