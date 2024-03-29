#pragma once
#include "../DirectX/DirectX.h"
class Fps {
	int mStartTime;         //測定開始時刻
	int mCount;             //カウンタ
	float mFps;             //fps
	static const int N = 60;//平均を取るサンプル数
	static const int FPS = 60;	//設定したFPS

public:
	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update() {
		if(mCount == 0) { //1フレーム目なら時刻を記憶
			mStartTime = GetNowTime(0);
		}
		if(mCount == N) { //60フレーム目なら平均を計算する
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
		int tookTime = GetNowTime(0) - mStartTime;	//かかった時間
		int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
		if(waitTime > 0) {
			Sleep(waitTime);	//待機
		}
	}
};

