#pragma once
#include "Bullet.h"
class PlayerBullet :
	public Bullet
{
public:
	PlayerBullet();
	virtual ~PlayerBullet();

	void start();
	void update();

private:
	void Draw();
	static list<shared_ptr<PlayerBullet>> Bullets;
};

