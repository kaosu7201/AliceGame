#pragma once
#include "../SpriteAnimation/AnimationManager.h"
#include "../SpriteAnimation/SpriteAnimation.h"
#include "../Collision/Collision.h"
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
	}
  D3DXVECTOR2 GetPos()
  {
    return position;
  }
	void SetPos(D3DXVECTOR2 vec) {
		position = vec;
	}
	bool GetFlag() const { return Flag; }
	bool GetStartFlag() const { return startFlag; }
	void SetStartFlag(bool flag) { startFlag=flag; }
	string getID() { return ObjID; }
protected:
	string ObjID;
	bool Flag;
	bool startFlag;

	D3DXVECTOR2 position; //ワールド座標
  D3DXVECTOR2 scPos;    //スクリーン座標
  D3DXVECTOR2 pivot;
  float vx, vy;

	AnimationController* AnimControll;
};
