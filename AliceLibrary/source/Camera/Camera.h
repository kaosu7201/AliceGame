#pragma once
#include "../DirectX/DirectX.h"

struct CameraInfo
{
  int ScreenWidth, ScreenHeight;
  float vx, vy, vz;
  D3DXVECTOR3 position;
  const D3DXVECTOR3* target;
  int rangeX, rangeY;
  bool _2DFlag;
  bool FollowFlag;
};

class Camera
{
public:
  Camera();
  virtual ~Camera();

  virtual void update() = 0;
  virtual void setScreenWH(int Width, int Height) = 0;
  virtual void setTargetPos(D3DXVECTOR3* pos) = 0;
  virtual void setRange(int x, int y) 
  { 
    Info.rangeX = x; 
    Info.rangeY = y;
  }
  CameraInfo getInfo() { return Info; }
protected:
  CameraInfo Info;
};

class Camera2D
  : public Camera
{
public:
  Camera2D();
  virtual ~Camera2D();
  
  void update();

  void setScreenWH(int Width, int Height)
  {
    Info.ScreenWidth = Width;
    Info.ScreenHeight = Height;
  }

  void setTargetPos(D3DXVECTOR3* pos)
  {
    Info.target = pos;
  }
};

