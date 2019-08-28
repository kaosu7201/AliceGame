#include "../Application/ApplicationBase.h"
#include "ObjectManager.h"
#include "../Camera/Camera.h"
Object::Object()
{
	Flag = true;
	startFlag = false;
}

void Object::scPosCalc()
{
  ApplicationBase *app = ApplicationBase::GetInstance();
  Camera* camera = app->GetCamera();
  CameraInfo info = camera->getInfo();
  float x = 0, y = 0, z = 0;
  if (info._2DFlag == true)
  {
    x = position.x - info.position.x + info.ScreenWidth / 2;
    y = position.y - info.position.y + info.ScreenHeight / 2;
  }
  scPos.x = x;
  scPos.y = y;
  scPos.z = z;
}
