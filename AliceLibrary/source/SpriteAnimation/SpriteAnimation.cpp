#include "SpriteAnimation.h"
#include "../Texture/DXTexture.h"
map<string, TexAnimePack> AnimPack;

SpriteAnimation::SpriteAnimation()
{
}


SpriteAnimation::~SpriteAnimation()
{
}

int pow2(int n)
{
  if (n < 0)
    return 0;
  if (n == 1)
    return 1;

  return (int)pow(2.0, (floor(log2(n - 1)) + 1.0));
}

bool SpriteAnimation::addAnimePack(string name, string FileName, int width, int height, int SplitNum)
{
	TexAnimePack buff;
	buff.width = width;
	buff.height = height;
	int width_num, height_num, split_num;
  D3DXIMAGE_INFO info;
  D3DXGetImageInfoFromFile(FileName.c_str(), &info);
	width_num = info.Width / width;
	height_num = info.Height / height;
	split_num = width_num * height_num;
  int w = pow2(width_num), h=pow2(height_num);
  if (TextureLoad(FileName.c_str(), &buff.texture, width*w, width*h))
	{
		buff.texture->GetLevelDesc(0, &buff.desc);
    buff.width_num = width_num;
    buff.height_num = height_num;
		if (split_num < SplitNum)
		{
			buff.SplitNum = split_num;
		}
		else
		{
			buff.SplitNum = SplitNum;
		}
		AnimPack[name] = buff;
		return true;
	}
	return false;
}


void SpriteAnimation::Draw(string name, int Index, float x,float y)
{
	Sprite sprite;
	if (Index >= AnimPack[name].SplitNum) {
		Index = AnimPack[name].SplitNum - 1;
	}
	sprite.setTexture(AnimPack[name].texture, false);
	sprite.setSize(AnimPack[name].width + 0, AnimPack[name].height + 0);
	int width = AnimPack[name].desc.Width / AnimPack[name].width;
	int height = AnimPack[name].desc.Height / AnimPack[name].height;
	sprite.setScreenSize(1280, 720);
  sprite.setUV((1.0f / width) * (Index % AnimPack[name].width_num), (1.0f / height) * (Index / AnimPack[name].width_num), (1.0f / width), (1.0f / height));
	sprite.setPriority(1.0f);
  //sprite.setScale(4.0f, 4.0f);
	sprite.setPos(x, y);
  sprite.draw();
	Sprite::drawAll();
	Sprite::clearDrawList();
}

