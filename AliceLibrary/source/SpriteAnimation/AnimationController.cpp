#include "../appframe.h"
#include "SpriteAnimation.h"


AnimationController::AnimationController()
{
	NowFrame = 0;
	endFrame = 0;
	SwitchFrame = 0;
	no = 0;
	name = "";
	AnimationManager::GetManager()->addObject(shared_ptr<AnimationController>(this));
}


AnimationController::~AnimationController()
{
}


void AnimationController::update()
{
	Calc();
}


void AnimationController::Draw()
{
	if (name != "") {
		SpriteAnimation::Draw(name, no, pos->x, pos->y);
	}
}


void AnimationController::Calc()
{
	int num = AnimPack[name].SplitNum;
	if (SwitchFrame != 0) {
		no = NowFrame / SwitchFrame;
		no %= num;
	}

	endFrame = num * SwitchFrame;

	NowFrame++;
	if (NowFrame >= endFrame) {
		NowFrame = 0;
	}
}