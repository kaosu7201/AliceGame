#pragma once
#include "../DirectX/DirectX.h"
#include "InputEnum.h"
#include "KeyBoardDevice.h"
#include "MouseDevice.h"

class _IController 
{
protected:
	IKeyInput m_keyboard;
	IMouseInput m_mouse;
	_IController(IKeyInput keyboard,IMouseInput mouse) :m_keyboard(keyboard),m_mouse(mouse) {}
public:
	virtual ~_IController() {}
	virtual void UpDate() = 0;

	//入力処理
	virtual bool GetButtonState(GAMECONTROLLER_BTN ctrl, STATE_TYPE get_type = PRESENT) = 0;
	virtual void GetRelativePos(double &x, double &y) = 0;
	virtual void GetAbsScreenPos(double &x, double &y) = 0;
	virtual void GetWheelMove(double &z) = 0;
	//キーコンフィグ
	virtual void SetDefault() = 0;
	virtual void SetConfigKeyBoard(GAMECONTROLLER_BTN ctrl, KEYBOARD_BUTTON key) = 0;
};
typedef _IController* IController;

struct ButtonSet
{
	KEYBOARD_BUTTON key;
	//GAMEPAD_BUTTON pad;
	MOUSE_BUTTON mouse;
};

class CDefaultController :public _IController
{
	typedef _IController Super;
private:
	ButtonSet m_Setting[GCBTN_NUM];
	static _IController* lpController;
public:
	CDefaultController(IKeyInput keyboard, IMouseInput mouse);
	~CDefaultController() {}
	static IController Create(IKeyInput keyboard, IMouseInput mouse);
	static _IController* GetController() { return lpController; }
	void UpDate();
  void Init();
	//入力処理
	bool GetButtonState(GAMECONTROLLER_BTN ctrl, STATE_TYPE get_type = PRESENT);
	void GetRelativePos(double &x, double &y);
	void GetAbsScreenPos(double &x, double &y);
	void GetWheelMove(double &z);
	//キーコンフィグ
	void SetDefault();
	void SetConfigKeyBoard(GAMECONTROLLER_BTN ctrl, KEYBOARD_BUTTON key);
};

void DrawDebugInfo();