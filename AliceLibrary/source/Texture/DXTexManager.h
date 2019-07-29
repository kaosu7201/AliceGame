#pragma once
#include <list>
#include <vector>
#include "../DirectX/DirectX.h"
#include "DXTexPack.h"

using namespace std;
class DXTexManager
{
public:
	DXTexManager();
	virtual ~DXTexManager();

	void CreateTexturePack(LPCSTR szName);
	void CreateTexture(LPCSTR szFileName);

	void Draw() {
		for (auto itr = _TexList.begin(), end_ = _TexList.end(); itr != end_; itr++) {
			(*itr)->Draw();
			RenderPack.clear();
		}
		_TexList.clear();
	}

	static DXTexManager *GetManagerInstance()
	{
		return _lpTexManager;
	}

	int GetTexPackHandleSize() {
		return TexPackHandle.size();
	}

	void SetTexPack(DXTexPack* Pack) {
		_TexList.push_back(Pack);
	}

  void AlldelList()
  {
    _TexList.clear();
  }

private:
	vector<int> TexPackHandle;
	list<DXTexture*> _TexList;
	static DXTexManager *_lpTexManager;
};

void DrawTexPack(int szIndex, int x, int y, int PackHandle);
void DrawTexPack(string szID, int x, int y, int PackHandle);
void DrawTexture(int handle, int x, int y);