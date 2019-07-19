/*
** OpenAL Sound Lib.
**
*/


/* 機能
OpenAL/OpenSL|ESによる演奏を行う
oggファイルをサポート
FILEからのストリーム演奏を行う(FILEより逐次読み出し。CPU負荷有り。メモリ負荷軽）
メモリからのストリーム演奏を行う(メモリより逐次読み出し。CPU負荷有り。メモリはoggのまま演奏するので軽い）
FILE/メモリからロードしstatic演奏を行う(読み出し時にメモリにPCMを置く。CPU負荷軽。メモリ使用量大）
演奏停止後、自動で削除されるPlayerを作れる。SEなど1ショット再生に有用。onTick()を適時呼ぶ必要あり
全体と個別のVolume/mute/pitch機能

*/

/* 実装サンプル

// Androidは以下を行う必要がある
LibSoundUtil::fileinit( AAssetManager * );	// 現在は PL_SetCurrentPath() でやるようにしてみている。

LibSound snd;
LibSoundBuffer *buf = snd.attach( "filename.ogg" );	// stream
LibSoundPlayer *ply = snd.player( buf );
ply->play();
while(1) {
	snd.onTick();	// 一定時間に一度呼ぶように
}
ply->stop();
snd.del( ply );
snd.del( buf );

*/



#ifndef	__LIBSOUND_H__
#define	__LIBSOUND_H__

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<memory.h>

#include	<list>
#include	<algorithm>


// windows
#ifdef	WIN32
	// open-al
	#include	<al/al.h>
	#include	<al/alc.h>

	// libs.
	#pragma comment(lib,"WinMM.lib")
	#pragma comment(lib,"OpenAL32.lib")
//	#pragma comment(lib,"libogg_static.lib")
//	#pragma comment(lib,"TremorLib.lib")

	// warning controll.
	#pragma warning( disable : 4996 4819 )

	#define		_CRTDBG_MAP_ALLOC
	#include	<malloc.h>
	#include	<crtdbg.h>					// メモリリーク検知用

	// thread
	#include	<process.h>

	// sleep
	#include	<Windows.h>

#endif


//ios
#ifdef	TARGET_OS_IPHONE
/*
OpenAL.framework, libogg.a, libtremor.a を導入する必要あり
*/
	// open-al
	#include	<OpenAL/al.h>
	#include	<OpenAL/alc.h>

	// thread
	#include	<pthread.h>

	// sleep
	#include	<unistd.h>

    // gettimeofday
    #include <sys/time.h>

#endif

#ifdef	_ANDROID_
/*
OpenSL|ESを使用する
*/
	#define	CODE_USE_OPENSL_ES

	// open-sl es
	#include <SLES/OpenSLES.h>
	#include <SLES/OpenSLES_Android.h>

	// thread
	#include	<pthread.h>

	// sleep
	#include	<unistd.h>

	// log10f
	#include <math.h>

//oglfw
//#include "../fwinclude.h"
	// android
	#include <jni.h>
	#include <android/log.h>
	#include <android/asset_manager_jni.h>



/*
sl参考
http://creative.dtiblog.com/?mode=m&no=760
http://raseene.asablo.jp/blog/2013/11/19/7061467
http://d.hatena.ne.jp/miujun/20120307

確認しおくべき
音量やプレーヤーの解放について
http://www.ku6.jp/keyword4/1.html

setpitch,volumeについて
http://creative.dtiblog.com/?mode=m&no=760

同時発音数について
http://vianote.blog.fc2.com/blog-category-1.html
*/


#endif

// ログ出力用制御
#ifndef	_LOGOUT
#define	_LOGOUT		printf
#endif
#ifndef	_LOGOUTSYS
#define	_LOGOUTSYS	printf
#endif


// OpenSL|ES用マクロ
#ifdef	CODE_USE_OPENSL_ES
	#define SL_Realize(a,b) (*(a))->Realize((a),(b))
	#define SL_GetInterface(a,b,c) (*(a))->GetInterface((a),(b),(c))
	#define SL_CreateAudioPlayer(a,b,c,d,e,f,g) (*(a))->CreateAudioPlayer((a),(b),(c),(d),(e),(f),(g))
	#define SL_CreateOutputMix(a,b,c,d,e) (*(a))->CreateOutputMix((a),(b),(c),(d),(e))
	#define SL_CreateVibraDevice(a,b,c,d,e,f) (*(a))->CreateVibraDevice((a),(b),(c),(d),(e),(f))
	#define SL_SetEnvironmentalReverbProperties(a,b) (*(a))->SetEnvironmentalReverbProperties((a),(b))
	#define SL_RegisterCallback(a,b,c) (*(a))->RegisterCallback((a),(b),(c))
	#define SL_Enqueue(a,b,c) (*(a))->Enqueue((a),(b),(c))
	#define SL_ClearQueue(a) (*(a))->Clear((a))
	#define SL_GetPlayState(a,b) (*(a))->GetPlayState((a),(b))
	#define SL_SetPlayState(a,b) (*(a))->SetPlayState((a),(b))
	#define SL_SetLoop(a,b,c,d) (*(a))->SetLoop((a),(b),(c),(d))
	#define SL_SetRate(a,b) (*(a))->SetRate((a),(b))
	#define SL_GetRateRange(a,b,c,d,e,f) (*(a))->GetRateRange((a),(b),(c),(d),(e),(f))
	#define SL_SetVolumeLevel(a,b) (*(a))->SetVolumeLevel((a),(b))
	#define SL_EnableStereoPosition(a,b) (*(a))->EnableStereoPosition((a),(b))
	#define SL_SetStereoPosition(a,b) (*(a))->SetStereoPosition((a),(b))
	#define SL_SetPosition(a,b,c) (*(a))->SetPosition((a),(b),(c))
	#define SL_GetPosition(a,b) (*(a))->GetPosition((a),(b))
	#define SL_Vibrate(a,b) (*(a))->Vibrate((a),(b))
	#define SL_Destroy(a) (*(a))->Destroy(a)
#endif


// libsound
#include	"libsoundutil.h"
#include	"libsoundstorage.h"
#include	"libsoundbuffer.h"
#include	"libsoundplayer.h"


using namespace std;

class LibSound
{

public:
#ifndef	CODE_USE_OPENSL_ES
	ALCdevice		*device;	// OpenAL - device
	ALCcontext		*context;	// OpenAL - context

#else
	SLObjectItf		engineObject;
	SLEngineItf		engineIf;
	SLObjectItf		mixerObject;
#endif

public:
	LibSound();
	~LibSound();

	// selfdeadを使う場合はこれを一定間隔で呼ぶ必要がある
	void onTick();

	LibSoundBuffer *attach( char *szFile );					// stream
	LibSoundBuffer *attach( void *pData, int nDataSize );	// stream
	LibSoundBuffer *load( char *szFile );					// static(file -> on memory pcm)
	LibSoundBuffer *load( void *pData, int nDataSize );		// static(memory -> on memory pcm)
	LibSoundPlayer *player( LibSoundBuffer *buf );

	list<LibSoundPlayer*>	list_player;
	list<LibSoundBuffer*>	list_buffer;

	void add( LibSoundPlayer *pl );
	void add( LibSoundBuffer *buf );
	void del( LibSoundPlayer *pl );
	void del( LibSoundBuffer *buf );

	// pitch, def:100
	int  pitch();
	void setpitch( int pitch );

	// volume, 0-100
	int  volume();
	void setvolume( int vol );

	// mute
	void setmute( bool mute );
	bool ismute();

	// pause
	void setpause( bool pause );
	bool ispause();


	// パラメータ
	int		m_pitch;
	int		m_vol;
	bool	m_mute;
	bool	m_pause;


private:

	LibSoundBuffer *_load( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize );
	LibSoundBuffer *_attach( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize );
	LibSoundStorage *_storageopen( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize, int *pcmdata_rate, int *pcmdata_bits, int *pcmdata_channels, LibSoundUtil::BUFFERFORMAT *format );


};



#endif
