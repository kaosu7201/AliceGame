#include "../Application/ApplicationBase.h"
#include "ObjectManager.h"
#include "../Camera/Camera.h"
Object::Object()
{
	Flag = true;
	startFlag = false;
  AnimControll = NULL;
}

void Object::scPosCalc()
{
  ApplicationBase *app = ApplicationBase::GetInstance();
  Camera* camera = app->GetCamera();
  CameraInfo info = camera->getInfo();
  float x = 0, y = 0, z = 0;
  if (info._2DFlag == true)
  {
    int imgW = 0, imgH = 0;
    if (AnimControll)
    {
      AnimControll->getImgWH(&imgW, &imgH);
    }
    x = position.x - info.position.x + info.ScreenWidth / 2;
    y = position.y - info.position.y + info.ScreenHeight / 2;
    scPos.x = x - imgW * pivot.x;
    scPos.y = y - imgH * pivot.y;
  }
}
