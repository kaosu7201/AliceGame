#pragma once
#include <vector>
#include <list>
#include <algorithm>
#include "../DirectX/DirectX.h"
#include "DXTexture.h"
#include "../FileNames/FileNames.h"

using namespace std;
class DXTexPack :
	public DXTexture
{
public:
	DXTexPack();
	DXTexPack(LPCSTR szName, int TexPackID);
	virtual ~DXTexPack();

	bool SetTexPack(const string &szFilePath);
	void Draw();

public:
	int PackID;
	vector<LPDIRECT3DTEXTURE9> _Texture;
	LIBTEXPACK_DATA *_TexPack;
	bool flag;
};


struct TexPack {
	int szIndex;
	string szID;
	int x, y;
};

extern list<TexPack> RenderPack;
extern vector<DXTexPack*> _TexPack;