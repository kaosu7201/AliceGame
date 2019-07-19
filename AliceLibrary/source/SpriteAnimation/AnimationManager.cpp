#include "AnimationManager.h"

AnimationManager *AnimationManager::AnimationMng = NULL;

AnimationManager::AnimationManager()
{
	AnimationMng = this;
}


AnimationManager::~AnimationManager()
{
}
