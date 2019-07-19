#include "Sprite.h"
class DXTexture
{
public:
	DXTexture();
	virtual ~DXTexture();

	virtual void Draw() = 0;

};

extern LPDIRECT3DTEXTURE9 pTexture;
