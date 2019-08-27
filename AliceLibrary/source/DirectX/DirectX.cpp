#include "DirectX.h"
#include "../Input/InputDevice.h"

LPD3DXFONT m_pFont = NULL;
// 現在時間取得関数
extern int GetPresentTime(void)
{
	return timeGetTime();
	//	return GetTickCount();
}

// プロセス制御変数
BOOL gg_bEndFlag = false;
int ProcessEventClac()
{
	int					nEventCnt;
	MSG					msg;

	//	if ( bLockFlag ) return( FALSE );
	if (gg_bEndFlag) return false;							// すでに終了か

	nEventCnt = 0;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!GetMessage(&msg, NULL, 0, 0)) {
			gg_bEndFlag = true;
			//			StopPcm();											// 音楽停止
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		nEventCnt++;
		if (nEventCnt >= MAX_EVENTS) break;
	}
	return true;
}


//メインループ
int ProcessEvent()
{
	int nResult;	//リザルトコード

	nResult = ProcessEventClac();	//イベント処理
	return nResult;
}


// 画面クリア
int HCls(void)
{
	int					nResult;								// リザルトコード
	DXDevice *appDevice = DXDevice::GetDevice();
				// 画面更新処理
	appDevice->pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 128), 1.0f, 0);
	return S_OK;
}


// 画面の更新
int WindowFlip(void)
{
	D3DRASTER_STATUS			rsRasterData;
	DXDevice *appDevice = DXDevice::GetDevice();
	if (gg_bEndFlag) return -26;						// 終了か

	// End the scene
	appDevice->pDevice->EndScene();
	//gg_bSceneActive = false;

	// ウィンドウモードならVBlank待ち
	//if (!gg_bFullMode) {
		do {													// VBlankになるまで待つ
			if (!ProcessEvent()) break;					// とりあえずイベント処理
			appDevice->pDevice->GetRasterStatus(0, &rsRasterData);
		} while (!rsRasterData.InVBlank);
	//}

	// 終わってなければ表示
	if (!gg_bEndFlag) {
		appDevice->pDevice->Present(NULL, NULL, NULL, NULL);		// バックバッファの内容を表示
		appDevice->pDevice->BeginScene();							// シーン開始
		//gg_bSceneActive = true;
	}
	else return -26;

	return S_OK;
}


int Flip(void)
{
	int					nResult;								// リザルトコード

	//// デバッグ情報の表示
	//if (gg_bDisplayDebug) {
	DrawDebugInfo();								// デバッグ情報表示
	//}

	nResult = WindowFlip();										// 画面更新処理

	//// デバッグキー・ポーズコントロール
	//if (gg_bDebugKey) {
	//	AB_ControlDebugPause();									// デバッグキー・ポーズコントロール
	//}
	//
	//fg_nFrameCount++;

	return nResult;
}


bool TextureLoad(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *pTexture) {
	DXDevice *appDevice = DXDevice::GetDevice();
	// 「テクスチャオブジェクト」の作成
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(pSrcFile, &info);
	if (FAILED(D3DXCreateTextureFromFileEx(appDevice->pDevice, pSrcFile, info.Width, info.Height, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000001, NULL, NULL, pTexture)))
	{
		MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
		return false;
	}
	return true;
}

bool TextureLoad(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *pTexture, int Width,int Height)
{
  DXDevice *appDevice = DXDevice::GetDevice();
  // 「テクスチャオブジェクト」の作成
  if (FAILED(D3DXCreateTextureFromFileEx(appDevice->pDevice, pSrcFile, Width, Height, 0, 0, D3DFMT_UNKNOWN,
    D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
    0xff000001, NULL, NULL, pTexture)))
  {
    MessageBox(0, "テクスチャの作成に失敗しました", "", MB_OK);
    return false;
  }
  return true;
}

HRESULT InitStringRender() {
	DXDevice *appDevice = DXDevice::GetDevice();
	//文字列レンダリングの初期化
	if (FAILED(D3DXCreateFont(appDevice->pDevice, 0, 10, FW_REGULAR, NULL, false, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "tahoma", &m_pFont)))
	{
		return E_FAIL;
	}
}

VOID RenderString(LPSTR szStr, INT iX, INT iY, int Color)
{
	RECT rect = { iX,iY,0,0 };
	//文字列のサイズを計算
	m_pFont->DrawTextA(NULL, szStr, -1, &rect, DT_CALCRECT, NULL);
	// そのサイズでレンダリング
	m_pFont->DrawText(NULL, szStr, -1, &rect, DT_LEFT | DT_BOTTOM, Color);
}

/**
* @brief 文字列を描画する関数
* @param[in] szStr 描画する文字列
* @param[in] iX 描画するX座標
* @param[in] iY 描画するY座標
*/
void DrawPrintf(int x, int y, int nColor, const char *fmt, ...) {
	char sz[0xffff];
	va_list list;
	va_start(list, fmt);
	vsprintf(sz, fmt, list);
	va_end(list);

	RenderString(sz, x, y, nColor);
}

int GetColor(int r, int g, int b) {
	return D3DCOLOR_ARGB(255, r, g, b);
}

int GetColor(int a, int r, int g, int b) {
	return D3DCOLOR_ARGB(a, r, g, b);
}


