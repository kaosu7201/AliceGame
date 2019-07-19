#pragma once
#include <vector>
using namespace std;

class DXInput
{
public:
	DXInput();
	virtual ~DXInput();
	bool CreateInput();
	LPDIRECTINPUT8    pDInput;
	LPDIRECTINPUTDEVICE8 m_KeyDevice;
	LPDIRECTINPUTDEVICE8 m_MouseDevice;
	static DXInput* GetInput() { return lpInput; }
private:
	static DXInput* lpInput;
};
