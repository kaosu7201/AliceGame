#include "Player.h"

Player::Player()
{
	ObjID = "Player";
	controller = CDefaultController::GetController();
	AnimControll = new AnimationController();
  AnimControll->pos = &scPos;
	_Collision = new CircleCollision(10, &position);
  direction = 1;
  ApplicationBase::GetInstance()->GetCamera()->setTargetPos(&position);
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
    AnimControll->PlayAnim("PlayerWalkL", 4);
    direction = -1;
    position.x -= 5.0f;
  }
  else if (controller->GetButtonState(GCBTN_RIGHT))
  {
    AnimControll->PlayAnim("PlayerWalkR", 4);
    direction = 1;
    position.x += 5.0f;
  }
  else
  {
    if (direction == 1)
    {
      AnimControll->PlayAnim("PlayerIdleR", 4);
      AnimControll->setFlag(true, true);
    }
    else
    {
      AnimControll->PlayAnim("PlayerIdleL", 4);
      AnimControll->setFlag(true, true);
    }
  }
}

list<shared_ptr<Object>> Player::GettableObject() 
{
	return list<shared_ptr<Object>>();
}

void Player::Draw()
{
  
}
