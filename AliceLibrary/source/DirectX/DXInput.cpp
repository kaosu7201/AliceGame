#include "DirectX.h"
#include "../Application/ApplicationBase.h"

DXInput *DXInput::lpInput = NULL;

DXInput::DXInput()
{
	lpInput = this;
}


DXInput::~DXInput()
{
}


BOOL WINAPI DIEnumDevCallback(
	LPCDIDEVICEINSTANCE lpddi,
	LPVOID pvRef
)
{
	// 第2引数がDIDEVICEINSTANCE構造体のvecor STLだとします
	vector<DIDEVICEINSTANCE>* ptr = (vector<DIDEVICEINSTANCE>*)pvRef;
	DIDEVICEINSTANCE tmp = *lpddi;

	// 配列に格納
	ptr->push_back(tmp);

	// 列挙を続ける場合はDIENUM_CONTINUE、止める場合はDIENUM_STOPを指定
	// 全ての列挙が終了すると自動的にコールバックも終了するので、
	// 止める理由が無ければDIENUM_CONTINUEにする。
	return DIENUM_CONTINUE;
}



// デバイスのオブジェクトを列挙するコールバック関数（static宣言）
BOOL WINAPI DIEnumDevObjCallback(
	LPCDIDEVICEOBJECTINSTANCE lpddoi,
	LPVOID pvRef
)
{
	// 第2引数をvector<DIDEVICEOBJECTINSTANCE>へのポインタに型変換
	vector<DIDEVICEOBJECTINSTANCE> *tmp = (vector<DIDEVICEOBJECTINSTANCE>*)pvRef;

	// 配列にオブジェクトの情報を格納
	tmp->push_back(*lpddoi);

	// 列挙は全て終われば自動的に終了するので、
	// 列挙し続ける
	return DIENUM_CONTINUE;
}


bool DXInput::CreateInput()
{
	HWND hWnd = DXDevice::GetDevice()->hWnd;
	HINSTANCE hInst = GetModuleHandle(0);
	HRESULT hr;
	hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&pDInput, NULL);
	if (FAILED(hr)) {
		return false;
	}
	//--------------------------------------------------------------------------------------
	//キーボード取得
	//--------------------------------------------------------------------------------------
	//キーボード
	pDInput->CreateDevice(GUID_SysKeyboard, &m_KeyDevice, NULL);
	//デバイスのフォーマット・強調レベルを設定
	m_KeyDevice->SetDataFormat(&c_dfDIKeyboard);
	m_KeyDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_KeyDevice->Acquire();

	//--------------------------------------------------------------------------------------
	//マウス取得
	//--------------------------------------------------------------------------------------
	//マウス
	pDInput->CreateDevice(GUID_SysMouse, &m_MouseDevice, NULL);
	//デバイスのフォーマット・強調レベルを設定
	m_MouseDevice->SetDataFormat(&c_dfDIMouse);
	m_MouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	m_MouseDevice->Acquire();
	return true;
}
