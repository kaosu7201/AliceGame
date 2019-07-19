#include "SpriteAnimation.h"
#include "../Texture/DXTexture.h"
map<string, TexAnimePack> AnimPack;

SpriteAnimation::SpriteAnimation()
{
}


SpriteAnimation::~SpriteAnimation()
{
}

bool SpriteAnimation::addAnimePack(string name, string FileName, int width, int height, int SplitNum)
{
	TexAnimePack buff;
	buff.width = width;
	buff.height = height;
	int width_num, height_num, split_num;
	if (TextureLoad(FileName.c_str(), &buff.texture))
	{
		buff.texture->GetLevelDesc(0, &buff.desc);
		width_num = buff.desc.Width / width;
		height_num = buff.desc.Height / height;
		split_num = width_num * height_num;
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
	sprite.setSize(AnimPack[name].width, AnimPack[name].height);
	int width = AnimPack[name].desc.Width / AnimPack[name].width;
	int height = AnimPack[name].desc.Height / AnimPack[name].height;
	sprite.setScreenSize(1280, 720);
	sprite.setUV((1.0f / width) * (Index % width), (1.0f / height) * (Index / width), (1.0f / width), (1.0f / height));
	sprite.setPriority(1.0f);
	sprite.setPos(x, y);
	sprite.draw();
	Sprite::drawAll();
	Sprite::clearDrawList();
}

