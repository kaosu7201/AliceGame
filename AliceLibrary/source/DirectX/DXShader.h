#pragma once
#include "DirectX.h"
#include <string>

using namespace std;
class DXShader
{
public:
	DXShader();
	virtual ~DXShader();

	ID3DXEffect *CreateShader(string FileName);
};
