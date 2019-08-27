#include "DirectX.h"
#include "../Input/InputDevice.h"

LPD3DXFONT m_pFont = NULL;
// ���ݎ��Ԏ擾�֐�
extern int GetPresentTime(void)
{
	return timeGetTime();
	//	return GetTickCount();
}

// �v���Z�X����ϐ�
BOOL gg_bEndFlag = false;
int ProcessEventClac()
{
	int					nEventCnt;
	MSG					msg;

	//	if ( bLockFlag ) return( FALSE );
	if (gg_bEndFlag) return false;							// ���łɏI����

	nEventCnt = 0;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if (!GetMessage(&msg, NULL, 0, 0)) {
			gg_bEndFlag = true;
			//			StopPcm();											// ���y��~
			return false;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		nEventCnt++;
		if (nEventCnt >= MAX_EVENTS) break;
	}
	return true;
}


//���C�����[�v
int ProcessEvent()
{
	int nResult;	//���U���g�R�[�h

	nResult = ProcessEventClac();	//�C�x���g����
	return nResult;
}


// ��ʃN���A
int HCls(void)
{
	int					nResult;								// ���U���g�R�[�h
	DXDevice *appDevice = DXDevice::GetDevice();
				// ��ʍX�V����
	appDevice->pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 128), 1.0f, 0);
	return S_OK;
}


// ��ʂ̍X�V
int WindowFlip(void)
{
	D3DRASTER_STATUS			rsRasterData;
	DXDevice *appDevice = DXDevice::GetDevice();
	if (gg_bEndFlag) return -26;						// �I����

	// End the scene
	appDevice->pDevice->EndScene();
	//gg_bSceneActive = false;

	// �E�B���h�E���[�h�Ȃ�VBlank�҂�
	//if (!gg_bFullMode) {
		do {													// VBlank�ɂȂ�܂ő҂�
			if (!ProcessEvent()) break;					// �Ƃ肠�����C�x���g����
			appDevice->pDevice->GetRasterStatus(0, &rsRasterData);
		} while (!rsRasterData.InVBlank);
	//}

	// �I����ĂȂ���Ε\��
	if (!gg_bEndFlag) {
		appDevice->pDevice->Present(NULL, NULL, NULL, NULL);		// �o�b�N�o�b�t�@�̓��e��\��
		appDevice->pDevice->BeginScene();							// �V�[���J�n
		//gg_bSceneActive = true;
	}
	else return -26;

	return S_OK;
}


int Flip(void)
{
	int					nResult;								// ���U���g�R�[�h

	//// �f�o�b�O���̕\��
	//if (gg_bDisplayDebug) {
	DrawDebugInfo();								// �f�o�b�O���\��
	//}

	nResult = WindowFlip();										// ��ʍX�V����

	//// �f�o�b�O�L�[�E�|�[�Y�R���g���[��
	//if (gg_bDebugKey) {
	//	AB_ControlDebugPause();									// �f�o�b�O�L�[�E�|�[�Y�R���g���[��
	//}
	//
	//fg_nFrameCount++;

	return nResult;
}


bool TextureLoad(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *pTexture) {
	DXDevice *appDevice = DXDevice::GetDevice();
	// �u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
	D3DXIMAGE_INFO info;
	D3DXGetImageInfoFromFile(pSrcFile, &info);
	if (FAILED(D3DXCreateTextureFromFileEx(appDevice->pDevice, pSrcFile, info.Width, info.Height, 0, 0, D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
		0xff000001, NULL, NULL, pTexture)))
	{
		MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "", MB_OK);
		return false;
	}
	return true;
}

bool TextureLoad(LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *pTexture, int Width,int Height)
{
  DXDevice *appDevice = DXDevice::GetDevice();
  // �u�e�N�X�`���I�u�W�F�N�g�v�̍쐬
  if (FAILED(D3DXCreateTextureFromFileEx(appDevice->pDevice, pSrcFile, Width, Height, 0, 0, D3DFMT_UNKNOWN,
    D3DPOOL_DEFAULT, D3DX_FILTER_NONE, D3DX_DEFAULT,
    0xff000001, NULL, NULL, pTexture)))
  {
    MessageBox(0, "�e�N�X�`���̍쐬�Ɏ��s���܂���", "", MB_OK);
    return false;
  }
  return true;
}

HRESULT InitStringRender() {
	DXDevice *appDevice = DXDevice::GetDevice();
	//�����񃌃��_�����O�̏�����
	if (FAILED(D3DXCreateFont(appDevice->pDevice, 0, 10, FW_REGULAR, NULL, false, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "tahoma", &m_pFont)))
	{
		return E_FAIL;
	}
}

VOID RenderString(LPSTR szStr, INT iX, INT iY, int Color)
{
	RECT rect = { iX,iY,0,0 };
	//������̃T�C�Y���v�Z
	m_pFont->DrawTextA(NULL, szStr, -1, &rect, DT_CALCRECT, NULL);
	// ���̃T�C�Y�Ń����_�����O
	m_pFont->DrawText(NULL, szStr, -1, &rect, DT_LEFT | DT_BOTTOM, Color);
}

/**
* @brief �������`�悷��֐�
* @param[in] szStr �`�悷�镶����
* @param[in] iX �`�悷��X���W
* @param[in] iY �`�悷��Y���W
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


