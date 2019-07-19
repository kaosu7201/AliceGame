#pragma once
#include "../DirectX/DirectX.h"
#include <map>

struct TexAnimePack
{
	int SplitNum;
	int width;
	int height;
	LPDIRECT3DTEXTURE9 texture;
	D3DSURFACE_DESC desc;
};


class SpriteAnimation
{
public:
	SpriteAnimation();
	virtual ~SpriteAnimation();

	static bool addAnimePack(string name, string FileName, int width, int height, int SplitNum);
	static void Draw(string name, int Index, float x = 0, float y = 0);
};

extern map<string, TexAnimePack> AnimPack;