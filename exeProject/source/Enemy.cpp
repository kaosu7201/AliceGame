#include "Enemy.h"



Enemy::Enemy()
{
  ObjID = "Enemy";
  AnimControll = new AnimationController();
  AnimControll->pos = &position;
  _Collision = new CircleCollision(10, &position);
  direction = 1;
}


Enemy::~Enemy() 
{

}

void Enemy::update()
{
  AnimControll->PlayAnim("EnemyIdle", 10);
}

void Enemy::start()
{
  position.x = 1280 / 3;
  position.y = 720 - 200;
}

void Enemy::Draw()
{

}


list<shared_ptr<Object>> Enemy::GetObjects() {
	return list<shared_ptr<Object>>();
}


list<shared_ptr<Object>> Enemy::GettableObject() {
	return list<shared_ptr<Object>>();
}
