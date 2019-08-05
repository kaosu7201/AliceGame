#include "Player.h"

Player::Player()
{
	ObjID = "Player";
	controller = CDefaultController::GetController();
	AnimControll = new AnimationController();
	AnimControll->pos = &position;
	_Collision = new CircleCollision(10, &position);
  direction = 1;
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
    direction = -1;
		position.x -= 3;
	}
	if (controller->GetButtonState(GCBTN_RIGHT))
	{
    direction = 1;
		position.x += 3;
	}

  if (direction == 1)
  {
    AnimControll->PlayAnim("PlayerIdleR", 10);
  }
  else
  {
    AnimControll->PlayAnim("PlayerIdleL", 10);
  }
}

list<shared_ptr<Object>> Player::GettableObject() 
{
	return list<shared_ptr<Object>>();
}

void Player::Draw()
{

}
