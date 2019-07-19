#pragma once
#include "../DirectX/DirectX.h"
#include <list>

class Sprite
{
public:
	Sprite();
	Sprite(int screenWidth, int screenHeight);
	Sprite(const Sprite &r);
	virtual ~Sprite();

	Sprite &operator =(const Sprite &r) { copy(r); return *this; }

	static void begin_first();
	static void end_last();
	static void clearDrawList();
	void setScreenSize(int w, int h);
	void setSize(int w, int h);
	void setPivot(float x, float y);
	void getPivot(float *x, float *y);
	void setPos(float x, float y);
	void getPos(float *x, float *y);
	void setRotate(float deg);
	float getRotate();
	void setScale(float sx, float sy);
	void getScale(float *sx, float *sy);
	void setTexture(IDirect3DTexture9 *tex, bool isResize);
	IDirect3DTexture9 *getTexture();
	void setUV( float left, float top, float width, float height );
	void getUV_LT(float *left, float *top);
	void getUV_WH(float *width, float *height);
	void setRGB(float r, float g, float b);
	void getRGB(float *r, float *g, float *b);
	void setAlpha(float a);
	float getAlpha();
	void setPriority(float z);
	float getPriority();
	void setActivity(bool isActive);
	bool getActivity();
	void draw();
	static void drawAll();

private:
	int scW,scH;
	int polyW,polyH;
	float pivotX,pivotY;
	float posX,posY,posZ;
	float rad;
	float scaleX,scaleY;
	IDirect3DTexture9 *tex;
	float uvLeft, uvTop, uvW, uvH;
	float r, g, b;
	float alpha;
	float priority;
	bool bActivity;

	static IDirect3DVertexBuffer9 *buf;			// 共通単位サイズ板ポリ頂点バッファ
	static std::list<Sprite*> drawObjectList;		// 描画対象リスト
	static IDirect3DVertexDeclaration9 *decl;	// 共通頂点宣言
	static ID3DXEffect *effect;					// 共通エフェクト

	// コピー
	void copy(const Sprite &r);
};

extern Sprite *pSprite;