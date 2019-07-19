#include "Player.h"

Player::Player()
{
	ObjID = "Player";
	controller = CDefaultController::GetController();
	AnimControll = new AnimationController();
	AnimControll->pos = &position;
	_Collision = new CircleCollision(10, &position);
}


Player::~Player()
{
}

void Player::start()
{
	position.x = 1280 / 2;
	position.y = 720 - 200;
}

void Player::update()
{
	if (controller->GetButtonState(GCBTN_LEFT))
	{
		position.x -= 3;
	}
	if (controller->GetButtonState(GCBTN_RIGHT))
	{
		position.x += 3;
	}
	AnimControll->PlayAnim("PlayerIdle", 20);
}

list<shared_ptr<Object>> Player::GettableObject() 
{
	return list<shared_ptr<Object>>();
}

void Player::Draw()
{

}
