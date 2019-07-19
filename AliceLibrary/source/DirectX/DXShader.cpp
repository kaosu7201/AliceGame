#include "DXShader.h"

DXShader::DXShader()
{
}


DXShader::~DXShader()
{
}


ID3DXEffect *DXShader::CreateShader(string FileName)
{
	ID3DXEffect *effect;
	ID3DXBuffer *error;
	DXDevice *appDevice = DXDevice::GetDevice();
	if (FAILED(D3DXCreateEffectFromFile(appDevice->pDevice, FileName.c_str(), 0, 0, 0, 0, &effect, &error))) {
		OutputDebugString((const char*)error->GetBufferPointer());
		return 0;
	}
	return effect;
}