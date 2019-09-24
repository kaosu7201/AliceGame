#include "InputDevice.h"

_IController *CDefaultController::lpController = NULL;

CDefaultController::CDefaultController(IKeyInput keyboard,IMouseInput mouse) :_IController(keyboard, mouse)
{
	lpController = this;
}

IController CDefaultController::Create(IKeyInput keyboard,IMouseInput mouse)
{
	return new CDefaultController(keyboard, mouse);
}

void CDefaultController::UpDate()
{
	m_keyboard->UpDate();
	m_mouse->UpDate();
}


void CDefaultController::Init()
{
  m_keyboard->Init();
}

/**
* @brief デフォルトコントローラー 
* @verbatim
* [引数]
*   ctrl     = 
*   get_type = 種別
*
* [戻り値]
*   true  : 
*   false :
* @endverbatim
*/
bool CDefaultController::GetButtonState(GAMECONTROLLER_BTN ctrl, STATE_TYPE get_type)
{
	bool key(false), mouse(false);
	if(m_keyboard != 0) {
		if(m_Setting[ctrl].key != (KEYBOARD_BUTTON)BUTTON_NONE) {
			key = m_keyboard->GetKeyState(m_Setting[ctrl].key, get_type);
		}
	}
	if (m_mouse != 0) {
		if (m_Setting[ctrl].mouse != (MOUSE_BUTTON)BUTTON_NONE) {
			mouse = m_mouse->GetButtonState(m_Setting[ctrl].mouse, get_type);
		}
	}
	return (key || mouse);
}

void CDefaultController::GetRelativePos(double & x, double & y)
{
	if (m_mouse != 0) {
		m_mouse->GetRelativePos(x, y);
	}
}

void CDefaultController::GetAbsScreenPos(double & x, double & y)
{
	if (m_mouse != 0) {
		m_mouse->GetAbsScreenPos(x, y);
	}
}

void CDefaultController::GetWheelMove(double & z)
{
	if (m_mouse != 0) {
		m_mouse->GetWheelMove(z);
	}
}

void CDefaultController::SetDefault()
{
	m_Setting[GCBTN_0].key = GC_KEY_Z;
	m_Setting[GCBTN_0].mouse = MB_BUTTON_0;

	m_Setting[GCBTN_1].key = GC_KEY_X;
	m_Setting[GCBTN_1].mouse = MB_BUTTON_1;

	m_Setting[GCBTN_2].key = GC_KEY_C;
	m_Setting[GCBTN_2].mouse = MB_BUTTON_2;

	m_Setting[GCBTN_3].key = GC_KEY_A;
	m_Setting[GCBTN_3].mouse = MB_BUTTON_3;

	m_Setting[GCBTN_4].key = GC_KEY_S;
	m_Setting[GCBTN_4].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_5].key = GC_KEY_D;
	m_Setting[GCBTN_5].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_6].key = GC_KEY_W;
	m_Setting[GCBTN_6].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_7].key = GC_KEY_Q;
	m_Setting[GCBTN_7].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_8].key = GC_KEY_E;
	m_Setting[GCBTN_8].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_9].key = GC_KEY_R;
	m_Setting[GCBTN_9].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_10].key = GC_KEY_O;
	m_Setting[GCBTN_10].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_11].key = GC_KEY_P;
	m_Setting[GCBTN_11].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_12].key = GC_KEY_SHIFT;
	m_Setting[GCBTN_12].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_13].key = GC_KEY_CTRL;
	m_Setting[GCBTN_13].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_14].key = GC_KEY_SPACE;
	m_Setting[GCBTN_14].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_15].key = GC_KEY_ESCAPE;
	m_Setting[GCBTN_15].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	ButtonSet none;
	none.key = (KEYBOARD_BUTTON)BUTTON_NONE;
	none.mouse = (MOUSE_BUTTON)BUTTON_NONE;

	for (int i = GCBTN_16; i < GCBTN_DOWN; ++i) {
		m_Setting[i] = none;
	}

	m_Setting[GCBTN_LEFT].key = GC_KEY_LEFT;
	m_Setting[GCBTN_LEFT].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_RIGHT].key = GC_KEY_RIGHT;
	m_Setting[GCBTN_RIGHT].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_UP].key = GC_KEY_UP;
	m_Setting[GCBTN_UP].mouse = (MOUSE_BUTTON)BUTTON_NONE;

	m_Setting[GCBTN_DOWN].key = GC_KEY_DOWN;
	m_Setting[GCBTN_DOWN].mouse = (MOUSE_BUTTON)BUTTON_NONE;
}

void CDefaultController::SetConfigKeyBoard(GAMECONTROLLER_BTN ctrl, KEYBOARD_BUTTON key)
{
	m_Setting[ctrl].key = key;
}


void DrawDebugInfo()
{
	_IController *Controller = CDefaultController::GetController();
	double x, y;
	Controller->GetRelativePos(x, y);
	DrawPrintf(0, 16, GetColor(0, 255, 0), "マウス相対座標:X %f:Y %f", x, y);
	Controller->GetAbsScreenPos(x, y);
	DrawPrintf(0, 32, GetColor(0, 255, 0), "マウス絶対座標:X %f:Y %f", x, y);

	for (int i = 0; i < GCBTN_NUM; i++)
	{
		DrawPrintf(0, 16 * i + 48, GetColor(0, 255, 0), "ボタン%d:%d", i, Controller->GetButtonState(GAMECONTROLLER_BTN(i)));
	}
}