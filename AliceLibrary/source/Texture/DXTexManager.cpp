#include "DXTexManager.h"
#include "../Application/Task.h"

DXTexManager *DXTexManager::_lpTexManager = NULL;

DXTexManager::DXTexManager()
{
	_lpTexManager = this;
}


DXTexManager::~DXTexManager()
{
	_TexList.clear();
}


void DXTexManager::CreateTexturePack(LPCSTR szName)
{
	_TexPack.push_back(new DXTexPack(szName, _TexList.size()));
}


void DrawTexPack(int szIndex, int x, int y, int PackHandle)
{
	Task *task = Task::getTask();
	if (_TexPack[PackHandle]->flag == false) {
		task->TexManage->SetTexPack(_TexPack[PackHandle]);
		_TexPack[PackHandle]->flag = true;
	}
	TexPack Pack;
	Pack.szIndex = szIndex;
	Pack.x = x;
	Pack.y = y;
	RenderPack.push_back(Pack);
}

void DrawTexPack(string szID, int x, int y, int PackHandle)
{
	Task *task = Task::getTask();
	if (_TexPack[PackHandle]->flag == false) {
		task->TexManage->SetTexPack(_TexPack[PackHandle]);
		_TexPack[PackHandle]->flag = true;
	}
	TexPack Pack;
	Pack.szID = szID;
	Pack.x = x;
	Pack.y = y;
	RenderPack.push_back(Pack);
}

void DrawTexture(int handle, int x, int y)
{
	Sprite::drawAll();
}