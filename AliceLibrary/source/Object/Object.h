#pragma once
#include "../SpriteAnimation/AnimationManager.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../Collision/CollisionShape.h"
class Object
{
public:
	Object();
	virtual ~Object() {}

	virtual void start() = 0;
	virtual void update() = 0;
	virtual void Draw() = 0;
  void scPosCalc();
	void SetPos(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}
	void SetPos(D3DXVECTOR3 vec) {
		position = vec;
	}
	bool GetFlag() const { return Flag; }
	bool GetStartFlag() const { return startFlag; }
	void SetStartFlag(bool flag) { startFlag=flag; }
	string getID() { return ObjID; }
	Collision2D* getCollision() { return _Collision; }
protected:
	string ObjID;
	bool Flag;
	bool startFlag;

	D3DXVECTOR3 position; //ワールド座標
  D3DXVECTOR3 scPos;    //スクリーン座標
  float vx, vy;

	AnimationController* AnimControll;
	Collision2D* _Collision;
};
