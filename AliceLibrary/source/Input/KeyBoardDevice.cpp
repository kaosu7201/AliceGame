#include "KeyBoardDevice.h"

void CDIKeyBoard::UpDate()
{
	HRESULT ret;
	LPDIRECTINPUTDEVICE8 KeyDevice = DXInput::GetInput()->m_KeyDevice;
	ZeroMemory(keys, sizeof(keys));
	ret = KeyDevice->GetDeviceState(sizeof(keys), keys);
	if(FAILED(ret)) {
		// Ž¸”s‚È‚çÄŠJ‚³‚¹‚Ä‚à‚¤ˆê“xŽæ“¾
		KeyDevice->Acquire();
		KeyDevice->GetDeviceState(sizeof(keys), keys);
	}
}

bool CDIKeyBoard::GetKeyState(KEYBOARD_BUTTON key, STATE_TYPE get_type)
{
	bool flag(false);
	switch(get_type)
	{
	case PRESENT:
		if(keys[key] & 0x80) {
			flag = true;
		}
		break;
	case JUST_DOWN:
		if ((keys[key] & 0x80) && !(olds[key] & 0x80)){
			flag = true;
		}
		break;
	case JUST_UP:
		if (!(keys[key] & 0x80) && (olds[key] & 0x80)) {
			flag = true;
		}
		break;
	default:
		break;
	}
	olds[key] = keys[key];
	return flag;
}

IKeyInput CDIKeyBoard::Create()
{
	return new CDIKeyBoard();
}
