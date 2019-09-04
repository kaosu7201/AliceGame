#include "../Application/ApplicationBase.h"
#include "Camera.h"

Camera::Camera()
{
  this->Info.target = NULL;
  ApplicationBase* app = ApplicationBase::GetInstance();
  this->Info.ScreenWidth = app->DispSizeW();
  this->Info.ScreenHeight = app->DispSizeH();
  this->Info.FollowFlag = true;
  this->Info._2DFlag = true;
  this->Info.vx = 0;
  this->Info.vy = 0;
  this->Info.vz = 0;
  this->Info.rangeX = app->DispSizeW();;
  this->Info.rangeY = app->DispSizeH();
}

Camera::~Camera()
{
}

Camera2D::Camera2D()
{

}


Camera2D::~Camera2D()
{
}

void Camera2D::update()
{
  if (Info.FollowFlag == true && Info.target != NULL)
  {
    Info.position = *Info.target;
  }
  else
  {
    Info.position.x += Info.vx;
    Info.position.y += Info.vy;
  }
  if (Info.position.x - Info.ScreenWidth / 2 < 0)
  {
    Info.position.x = 0 + Info.ScreenWidth / 2;
  }
  if (Info.position.y - Info.ScreenHeight / 2 < 0)
  {
    Info.position.y = 0 + Info.ScreenHeight / 2;
  }
  if (Info.position.x + Info.ScreenWidth / 2 > Info.rangeX)
  {
    Info.position.x = Info.rangeX - Info.ScreenWidth / 2;
  }
  if (Info.position.y + Info.ScreenHeight / 2 > Info.rangeY)
  {
    Info.position.y = Info.rangeY - Info.ScreenHeight / 2;
  }
}

