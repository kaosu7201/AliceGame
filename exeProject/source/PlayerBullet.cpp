#include "PlayerBullet.h"

list<shared_ptr<PlayerBullet>> PlayerBullet::Bullets;

PlayerBullet::PlayerBullet()
{
	Bullets.push_back(shared_ptr<PlayerBullet>(this));
}


PlayerBullet::~PlayerBullet()
{

}


void PlayerBullet::start()
{

}


void PlayerBullet::update()
{

}


void PlayerBullet::Draw()
{

}
