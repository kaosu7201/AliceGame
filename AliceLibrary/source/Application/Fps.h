#pragma once
#include "../DirectX/DirectX.h"
class Fps {
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	static const int N = 60;//���ς����T���v����
	static const int FPS = 60;	//�ݒ肵��FPS

public:
	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update() {
		if(mCount == 0) { //1�t���[���ڂȂ玞�����L��
			mStartTime = GetNowTime(0);
		}
		if(mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
			int t = GetNowTime(0);
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
		return true;
	}

	void Draw() {
		DrawPrintf(0, 0, GetColor(255, 255, 255), "%.1f", mFps);
	}

	void Wait() {
		int tookTime = GetNowTime(0) - mStartTime;	//������������
		int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
		if(waitTime > 0) {
			Sleep(waitTime);	//�ҋ@
		}
	}
};

