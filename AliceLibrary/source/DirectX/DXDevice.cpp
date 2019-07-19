#include "DirectX.h"
#include "../Application/ApplicationBase.h"

DXDevice *DXDevice::lpDevice = NULL;
DXDevice *DirectDevice;


DXDevice::DXDevice()
{
	lpDevice = this;
}


DXDevice::~DXDevice()
{
	SAFE_DELETE(pDevice);
	SAFE_DELETE(pD3D);
	SAFE_DELETE(lpDXInput);
}


bool DXDevice::Init(HINSTANCE hInstance)
{
	if (!WindowInit(hInstance)) {
		return false;
	}
	if (!DirectInit()) {
		return false;
	}
	hInst = hInstance;
	lpDXInput = new DXInput;
	lpDXInput->CreateInput();
}


#define WIN_CLASS_NAME "AliceWindow"
bool DXDevice::WindowInit(HINSTANCE hInstance)
{
	ApplicationBase *appBase = ApplicationBase::GetInstance();
	WNDCLASSEX			wc;
	// �E�B���h�E�N���X�\���̂̐ݒ�
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL; //LoadIcon( hInstance, "PIXY_ICON" );
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL; //NAME;
	wc.lpszClassName = WIN_CLASS_NAME;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hIconSm = NULL; //LoadIcon( hInstance, "PIXY_S_ICON" );

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(						// �E�B���h�E�̐���
		WS_EX_OVERLAPPEDWINDOW,
		WIN_CLASS_NAME, "AliceGame",
		WS_OVERLAPPED | WS_SYSMENU,
		0, 0,
		appBase->DispSizeW(), appBase->DispSizeH(),
		NULL, NULL,
		hInstance,
		NULL);

	if (!hWnd) {
		return hWnd;
	}

	RECT wRect, cRect;  // �E�B���h�E�S�̂̋�`�A�N���C�A���g�̈�̋�`
	int ww, wh;         // �E�B���h�E�S�̂̕��A����
	int cw, ch;         // �N���C�A���g�̈�̕��A����

	// �E�B���h�E�S�̂̕��E�������v�Z
	GetWindowRect(hWnd, &wRect);
	ww = wRect.right - wRect.left;
	wh = wRect.bottom - wRect.top;
	// �N���C�A���g�̈�̕��E�������v�Z
	GetClientRect(hWnd, &cRect);
	cw = cRect.right - cRect.left;
	ch = cRect.bottom - cRect.top;
	// �N���C�A���g�̈�ȊO�ɕK�v�ȃT�C�Y���v�Z
	ww = ww - cw;
	wh = wh - ch;
	// �E�B���h�E�S�̂ɕK�v�ȃT�C�Y���v�Z
	ww = appBase->DispSizeW() + ww;
	wh = appBase->DispSizeH() + wh;

	// �v�Z�������ƍ������E�B���h�E�ɐݒ�
	SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);							// �E�B���h�E�̕\��
	SetFocus(hWnd);								// �t�H�[�J�X�̃Z�b�g

	return true;
}


bool DXDevice::DirectInit()
{
	// IDirect3D9�I�u�W�F�N�g�̍쐬
	if ((pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		return false;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	// Device�̐ݒ�
	d3dpp.Windowed = true;
	d3dpp.hDeviceWindow = NULL;
	d3dpp.BackBufferWidth = 1280;
	d3dpp.BackBufferHeight = 720;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferCount = 1;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	// VSYNC�҂��L��)
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	// Direct3D�f�o�C�X�̐���
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&pDevice)))
	{
		SAFE_DELETE(pD3D);
		return false;
	}
	if (FAILED(InitStringRender()))
	{
		return false;
	}
	return true;
}

// �L�[���͑҂��E���̓L�[�擾�֐�
extern int GetNowTime(int nDummy)
{
	int							nResult;

	nResult = GetPresentTime();

	return nResult;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return(DefWindowProc(hwnd, msg, wParam, lParam));
	}
	return (0L);
}