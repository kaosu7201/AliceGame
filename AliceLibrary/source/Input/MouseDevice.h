#pragma once
#include "InputEnum.h"

class IMouseInputDevice
{
public:
	IMouseInputDevice() {}
	virtual ~IMouseInputDevice() {}
	virtual void UpDate() = 0;

	virtual bool GetButtonState(MOUSE_BUTTON button, STATE_TYPE get_type = PRESENT) = 0;
	virtual bool GetRelativePos(double &x, double &y) = 0;
	virtual bool GetAbsScreenPos(double &x, double &y) = 0;
	virtual bool GetWheelMove(double &z) = 0;
};
typedef IMouseInputDevice* IMouseInput;

class CDIMouse :
	public IMouseInputDevice
{
public:
	CDIMouse() {}
	virtual ~CDIMouse() {}
	void UpDate();

	bool GetButtonState(MOUSE_BUTTON button, STATE_TYPE get_type = PRESENT);
	bool GetRelativePos(double &x, double &y);
	bool GetAbsScreenPos(double &x, double &y);
	bool GetWheelMove(double &z);

	static IMouseInput Create();

private:
	DIMOUSESTATE2 dims;
	BYTE oldsrgbButton[8];
};

