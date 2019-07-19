#ifndef	__LIBSOUNDPLAYER_H__
#define	__LIBSOUNDPLAYER_H__

class LibSoundPlayer
{
public:
	LibSound	*m_libsnd;

#ifndef	CODE_USE_OPENSL_ES
	ALuint			source;
#else
	SLObjectItf		sl_source;

	SLPlayItf						sl_ifPlayer;		//mState;
	SLAndroidSimpleBufferQueueItf	sl_ifQBuffer;		//mQBuffer;
	SLEffectSendItf					sl_ifEffectSend;	//mEffectSend;
	SLPlaybackRateItf				sl_ifRate;			//mRate;
	SLVolumeItf						sl_ifVolume;		//mVolume;
//	SLSeekItf						sl_ifSeek;

	SLpermille		sl_rateMin, sl_rateMax, sl_rateStepSize;
	SLuint32		sl_rateCapabilities;

	static void _slCallbackPlayer( SLAndroidSimpleBufferQueueItf, void *context );
	void slCallbackPlayer();
#endif

	LibSoundBuffer	*buf;

	LibSoundPlayer(LibSound*);
	~LibSoundPlayer();

	// open-al rapper
	void alCreatePlayer();
	void alDeletePlayer();
	void alPlay();
	void alStop();
	void alPause();
	void alSetCursor( int pos );
	int alGetCursor();
	void alSetVolume( float vol );
	void alSetPitch( float pitch );
	bool alIsPlay();
	bool alIsPause();


	// controller
	void play();
	void stop();
	void stop( bool nodead, bool destract );
	bool isplay();
	void pause();
	bool ispause();

	// loop
	int loopcnt();
	void setloop( int loopcnt );	//0:�������[�v, 1:1��̂�, 2�`:�X�g���[�����̃��[�v�J�E���g
	int loopstart();
	void setloopstart( int start );
	int looplength();
	void setlooplength( int length );

	// pitch, def:100
	int  pitch();
	void setpitch( int pitch );
	void setpitch();

	// volume, 0-100
	int  volume();
	void setvolume( int vol );
	void setvolume();

	// mute
	bool ismute();
	void setmute( bool mute );
	void setmute();

	// sample cursor
	int  cursor();
	void setcursor( int cursor );

	// delete this.
	bool stopdead();
	void setstopdead( bool dead );

	// �p�����[�^
	int		m_pitch;
	int		m_vol;
	bool	m_mute;
	bool	m_stopdead;

	// ���[�v
	int		m_start_cur;	// �Đ��J�n�ʒu
	int		m_loop_cnt;		// ���[�v�c��񐔁B0�̏ꍇ�͖������[�v
	int		m_loop_start;	// ���[�v�ʒu�X�^�[�g(sample�l)
	int		m_loop_length;	// ���[�v�̒���(sample�l)

	// �X�g���[���p�f�[�^
	bool	m_stream_first;
	int		m_load_sample;	// �ǂݍ���ł���sample�l��
	int		m_load_sample_base[LibSoundUtil::BUFFER_SIZE];	// currentpos�p


	// ���Ԋu�ŌĂ΂��(selfdead�p)
	void	onTick();


	// thread�֘A�̒�`
#ifndef	CODE_USE_OPENSL_ES
	// �X�g���[���Đ��X���b�h�p
	void _play_thread( void *pParam );
	bool	m_play_thread_run;
	bool	m_play_thread_finish;

	#ifdef	WIN32
		#define THREADCALLFUNC	unsigned int __stdcall
		#define	THREADCALL_PLAY_THREAD()	{ unsigned dummy; _beginthreadex( NULL, 0, threadexec_play_thread, (void*)this, 0, &dummy ); }
		#define	THREADSTOP_PLAY_THREAD()	
	#endif
	#ifdef	TARGET_OS_IPHONE
		#define THREADCALLFUNC	void *
			pthread_t		m_thread_play_thread, m_thread_isplay_thread;
		#define	THREADCALL_PLAY_THREAD()	{ pthread_create( &m_thread_play_thread, NULL, threadexec_play_thread, (void*)this ); }
		#define	THREADSTOP_PLAY_THREAD()	{ if ( m_thread_play_thread ) { pthread_join( m_thread_play_thread, NULL ); } m_thread_play_thread = 0; }
	#endif

	static THREADCALLFUNC threadexec_play_thread( void*args ){ reinterpret_cast<LibSoundPlayer*>(args)->_play_thread( args ); return 0; }

#else

	bool	sl_nextlast;
	bool	sl_play_run;

#endif

	bool	_play_static;


private:
	void selfdead();
	void _play_stream_first();
	int _que( int processed, void *quelist, int *pprocesscnt );
	int _quepost( int processcnt, void *quelist );
	void _play_stream_finish();


};



#endif
