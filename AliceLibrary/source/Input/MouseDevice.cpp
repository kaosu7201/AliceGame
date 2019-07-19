#include "MouseDevice.h"

void CDIMouse::UpDate()
{
	HRESULT ret;
	LPDIRECTINPUTDEVICE8 MouseDevice = DXInput::GetInput()->m_MouseDevice;
	ret = MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &dims);
	if (FAILED(ret)) {
		MouseDevice->Acquire();
		MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &dims);
	}
}

bool CDIMouse::GetButtonState(MOUSE_BUTTON button, STATE_TYPE get_type)
{
	bool flag(false);
	switch (get_type)
	{
	case PRESENT:
		if (dims.rgbButtons[button] & 0x80) {
			flag = true;
		}	
		break;
	case JUST_DOWN:
		if ((dims.rgbButtons[button] & 0x80) && !(oldsrgbButton[button] & 0x80)) {
			flag = true;
		}
		break;
	case JUST_UP:
		if (!(dims.rgbButtons[button] & 0x80) && (oldsrgbButton[button] & 0x80)) {
			flag = true;
		}
		break;
	default:
		break;
	}
	oldsrgbButton[button] = dims.rgbButtons[button];
	return flag;
}

bool CDIMouse::GetRelativePos(double & x, double & y)
{
	x = dims.lX;
	y = dims.lY;
	return true;
}

bool CDIMouse::GetAbsScreenPos(double & x, double & y)
{
	HWND hWnd = DXDevice::GetDevice()->hWnd;
	POINT po;
	// マウスの位置を取得
	GetCursorPos(&po);
	ScreenToClient(hWnd, &po);
	x = po.x;
	y = po.y;
	return true;
}

bool CDIMouse::GetWheelMove(double & z)
{
	z = dims.lZ;
	return true;
}

IMouseInput CDIMouse::Create()
{
	return new CDIMouse();
}
