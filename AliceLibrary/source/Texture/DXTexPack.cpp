#include <stdlib.h>
#include "DXTexPack.h"
#include <iostream>


list<TexPack> RenderPack;
vector<DXTexPack*> _TexPack;

DXTexPack::DXTexPack()
{
}


DXTexPack::DXTexPack(LPCSTR szName, int TexPackID)
{	
	list<string> FilePath;
	FilePath = GetTexPackFilePathInDir(szName);
	for (auto itr = FilePath.begin(), end_ = FilePath.end(); itr != end_; itr++) 
	{
		if (itr->find("bin") != string::npos)
		{
			SetTexPack(itr->c_str());
		}
		else
		{
			LPDIRECT3DTEXTURE9 Texture;
			if (TextureLoad(itr->c_str(), &Texture))
			{
				LibTexPackSetTextureId(_TexPack, _Texture.size(), _Texture.size());
				_Texture.push_back(Texture);
			}
		}
	}
	PackID = TexPackID;
	flag = false;
}


DXTexPack::~DXTexPack()
{
	// 解放
	_Texture.clear();
	LibTextPackReleaseBin(_TexPack);
	LibTexPackRelease();
}


struct TexInfo {	
	int tex;
	int sx, sy, sw, sh;
	int ox, oy, ow, oh;
	int dx, dy;
};


void SetTexInfo(TexInfo &out, const list<TexPack>::iterator itr, const DXTexPack *pPack)
{
	TexInfo Info;
	if ((*itr).szID != "") {
		LibTexPackGetData(pPack->_TexPack, (*itr).szID.c_str(),
			&Info.tex, &Info.sx, &Info.sy, &Info.sw, &Info.sh,
			&Info.ox, &Info.oy, &Info.ow, &Info.oh);
	}
	else {
		LibTexPackGetData(pPack->_TexPack, (*itr).szIndex,
			&Info.tex, &Info.sx, &Info.sy, &Info.sw, &Info.sh,
			&Info.ox, &Info.oy, &Info.ow, &Info.oh);
	}
	Info.dx = (*itr).x;
	Info.dy = (*itr).y;
	out = Info;
}


void RenderSprite(DXTexPack *pPack) 
{
	D3DSURFACE_DESC desc;
	for (int i = 0; i < LibTexPackGetTextureNum(pPack->_TexPack); i++) {
		// テクスチャの指定
		//appDevice->pDevice->SetTexture(0, );
		for (auto itr = RenderPack.begin(), end_ = RenderPack.end(); itr != end_; itr++) {
			Sprite &sprite = *new Sprite();
			pPack->_Texture[i]->GetLevelDesc(0, &desc);
			sprite.setTexture(pPack->_Texture[i], false);
			TexInfo Info;
			SetTexInfo(Info, itr, pPack);
			if (pPack->PackID || Info.tex != i) {
				continue;
			}
			sprite.setSize(Info.sw, Info.sh);
			sprite.setScreenSize(1280, 720);
			sprite.setUV((Info.sx / 1280.0f) / (desc.Width - 1) * 1280, (Info.sy / 720.0f) / (desc.Height-1) * 720,
				(Info.sw / 1280.f) / (desc.Width - 1) * 1280, (Info.sh / 720.0f) / (desc.Height - 1) * 720);
			sprite.setPriority(1.0f);
			sprite.setPos(Info.dx, Info.dy);
			sprite.draw();
		}
	}
	Sprite::drawAll();
	Sprite::clearDrawList();
	pPack->flag = false;
}


void DXTexPack::Draw()
{
	RenderSprite(this);
}


bool DXTexPack::SetTexPack(const string &szFilePath) 
{
	// binファイルの読み込み
	FILE *fp;
	fp = fopen(szFilePath.c_str(), "rb");
	if (!fp)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END); int fsize = ftell(fp); fseek(fp, 0, SEEK_SET);
	void *p = malloc(fsize);
	fread(p, 1, fsize, fp);
	fclose(fp);

	// 登録
	_TexPack = LibTexPackSetBin(p, fsize);
	free(p);
	return true;
}