#pragma once
class AnimationController
{
public:
	AnimationController();
	virtual ~AnimationController();
	void update();

	void Draw();
	D3DXVECTOR3* pos;

	void PlayAnim(string name, int SwFrame) {
		if (this->name != name) {
			NowFrame = 0;
			setAnimName(name);
			setSwitchFrame(SwFrame);
		}
	}
	void setAnimName(string name) { this->name = name; }
	void setSwitchFrame(int a) { SwitchFrame = a; }
private:
	void Calc();

	string name;

	bool playflag;
	int NowFrame, endFrame;		//�o�߃t���[���ƏI���t���[��
	int SwitchFrame;			//�\���t���[����
	int no;
};

