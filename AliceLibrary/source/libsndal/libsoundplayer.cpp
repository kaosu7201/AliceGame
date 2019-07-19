#include "libsound.h"


LibSoundPlayer::LibSoundPlayer(LibSound* libsnd) {
	m_libsnd = libsnd;
	m_stream_first = false;
	for ( int i = 0; i < LibSoundUtil::BUFFER_SIZE; i++ ) { m_load_sample_base[i] = 0; }
	m_start_cur = 0;
	m_pitch = 100;
	m_vol = 100;
	m_mute = false;
	m_stopdead = false;
	m_loop_cnt = 1;	// ループ残り回数。0以下の場合は無限ループ
	m_loop_start = 0;
//	m_loop_length = (44100*60*60);		// 60min.
	m_loop_length = 0;		// 60min.
	buf = NULL;

#ifndef	CODE_USE_OPENSL_ES
	source = 0;
	m_play_thread_run = false;
	m_play_thread_finish = false;
	#ifdef	TARGET_OS_IPHONE
		m_thread_play_thread = 0;
	#endif
#else
	sl_source = NULL;
	sl_nextlast = false;
	sl_play_run = false;
#endif
	_play_static = false;
}

LibSoundPlayer::~LibSoundPlayer() {
	stop(true,true);
	alDeletePlayer();
	m_libsnd->del( this );
}


void LibSoundPlayer::alCreatePlayer() {
#ifndef	CODE_USE_OPENSL_ES
	alGenSources( 1, &source ); _CHECK_AL_ERROR();
#else
	_LOGOUTSYS("call: alCreatePlayer");
	// プレイヤの作成
	SLresult slResult = 0;
	{
		SLDataFormat_PCM	sl_format;

		// ここまでにbufフォーマットが決まってる必要がある
		
		sl_format.formatType	= SL_DATAFORMAT_PCM;
		sl_format.numChannels	= buf->channel();
		sl_format.samplesPerSec	= buf->bitrate() * 1000;
		sl_format.bitsPerSample	= buf->bits();
		sl_format.containerSize	= buf->bits();
		sl_format.channelMask	= (sl_format.numChannels == 2) ? (SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT) : (SL_SPEAKER_FRONT_CENTER);
		sl_format.endianness	= SL_BYTEORDER_LITTLEENDIAN;

		SLDataLocator_AndroidSimpleBufferQueue sl_queue = { SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, 2 };
		SLDataSource src = { &sl_queue, &sl_format };
		SLDataLocator_OutputMix outmix = { SL_DATALOCATOR_OUTPUTMIX, m_libsnd->mixerObject };
		SLDataSink audioSnk = { &outmix, NULL };
		const SLInterfaceID ids[] = {
			SL_IID_PLAY, 
			SL_IID_BUFFERQUEUE, 
			SL_IID_EFFECTSEND, 
			SL_IID_PLAYBACKRATE, 
			SL_IID_VOLUME,
//			SL_IID_SEEK,		// SEEKインタフェイスの取得が成功しない..
		};
		const SLboolean req[] = {
			SL_BOOLEAN_TRUE,
			SL_BOOLEAN_TRUE,
			SL_BOOLEAN_TRUE,
			SL_BOOLEAN_TRUE,
			SL_BOOLEAN_TRUE,
//			SL_BOOLEAN_TRUE,
		};
		slResult = SL_CreateAudioPlayer( m_libsnd->engineIf, &sl_source, &src, &audioSnk, sizeof(ids)/sizeof(ids[0]), ids, req ); _CHECK_SL_ERROR();
		slResult = SL_Realize( sl_source, SL_BOOLEAN_FALSE ); _CHECK_SL_ERROR();
	}
	// 各種インタフェイス作成
	slResult = SL_GetInterface( sl_source, SL_IID_PLAY, &sl_ifPlayer ); _CHECK_SL_ERROR();
	slResult = SL_GetInterface( sl_source, SL_IID_BUFFERQUEUE, &sl_ifQBuffer); _CHECK_SL_ERROR();
	slResult = SL_GetInterface( sl_source, SL_IID_EFFECTSEND, &sl_ifEffectSend ); _CHECK_SL_ERROR();
	slResult = SL_GetInterface( sl_source, SL_IID_PLAYBACKRATE, &sl_ifRate ); _CHECK_SL_ERROR();
	slResult = SL_GetInterface( sl_source, SL_IID_VOLUME, &sl_ifVolume ); _CHECK_SL_ERROR();
//	slResult = SL_GetInterface( sl_source, SL_IID_SEEK, &sl_ifSeek ); _CHECK_SL_ERROR();
	
	// rateのmin, maxを取得
	slResult = SL_GetRateRange( sl_ifRate, 0, &sl_rateMin, &sl_rateMax, &sl_rateStepSize, &sl_rateCapabilities ); _CHECK_SL_ERROR();

	// 再生コールバック登録
	slResult = SL_RegisterCallback(sl_ifQBuffer, _slCallbackPlayer, this); _CHECK_SL_ERROR();

#endif
}

void LibSoundPlayer::alDeletePlayer() {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alDeleteSources(1, &source); _CHECK_AL_ERROR(); source = 0; }
#else
	_LOGOUTSYS("call: alDeletePlayer");
	SLresult slResult = 0;
	if ( sl_source ) { alStop(); SL_Destroy( sl_source ); _CHECK_SL_ERROR(); sl_source = NULL; }
#endif
}

void LibSoundPlayer::alPlay() {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alSourcePlay(source); _CHECK_AL_ERROR(); _play_static = true; }
#else
	_LOGOUTSYS("call: alPlay");
	SLresult slResult = 0;
	if ( sl_source ) { slResult = SL_SetPlayState( sl_ifPlayer, SL_PLAYSTATE_PLAYING ); _CHECK_SL_ERROR(); _play_static = true; }
#endif
}

void LibSoundPlayer::alStop() {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alSourceStop(source); _CHECK_AL_ERROR(); _play_static = false; }
#else
	_LOGOUTSYS("call: alStop");
	SLresult slResult = 0;
	if ( sl_source ) {
		slResult = SL_SetPlayState( sl_ifPlayer, SL_PLAYSTATE_STOPPED ); _CHECK_SL_ERROR();
		slResult = SL_ClearQueue( sl_ifQBuffer ); _CHECK_SL_ERROR();
		_play_static = false;
	}
#endif
}

void LibSoundPlayer::alPause() {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alSourcePause(source); _CHECK_AL_ERROR(); _play_static = false; }
#else
	_LOGOUTSYS("call: alPause");
	SLresult slResult = 0;
	if ( sl_source ) { slResult = SL_SetPlayState( sl_ifPlayer, SL_PLAYSTATE_PAUSED ); _CHECK_SL_ERROR(); _play_static = false; }
#endif
}

bool LibSoundPlayer::alIsPlay() {
#ifndef	CODE_USE_OPENSL_ES
	ALint state;
	alGetSourcei(source,AL_SOURCE_STATE,&state); _CHECK_AL_ERROR();
	if ( state == AL_PLAYING ) {
		return true;
	}
	if ( _play_static ) { 
		return true;
	}
	return false;
#else
//	_LOGOUTSYS("call: alIsPlay");
	SLresult slResult = 0;
	SLuint32 state = 0;
	if ( sl_source ) { slResult = SL_GetPlayState( sl_ifPlayer, &state ); _CHECK_SL_ERROR(); }
	if ( state == SL_PLAYSTATE_PLAYING ) {
		return true;
	}
	if ( _play_static ) { 
		return true;
	}
	return false;
#endif
}

bool LibSoundPlayer::alIsPause() {
#ifndef	CODE_USE_OPENSL_ES
	ALint state;
	alGetSourcei(source,AL_SOURCE_STATE,&state); _CHECK_AL_ERROR();
	if ( state == AL_PAUSED ) {
		return true;
	}
	return false;
#else
//	_LOGOUTSYS("call: alIsPause");
	SLresult slResult = 0;
	SLuint32 state = 0;
	if ( sl_source ) { slResult = SL_GetPlayState( sl_ifPlayer, &state ); _CHECK_SL_ERROR(); }
	return ( state == SL_PLAYSTATE_PAUSED );
#endif
}



void LibSoundPlayer::alSetCursor( int pos ) {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alSourcei(source, AL_SAMPLE_OFFSET, pos); _CHECK_AL_ERROR(); }
#else
	_LOGOUTSYS("call: alSetCursor" );
	SLresult slResult = 0;
	// ミリ秒で指定する必要があるので、Sample単位から変換
	SLmillisecond ms = pos * 1000 / ( buf->bitrate() * ( buf->bits() / 8 ) * buf->channel() );
//	if ( sl_source ) { slResult = SL_SetPosition( sl_ifSeek, ms, SL_SEEKMODE_FAST ); _CHECK_SL_ERROR(); }
#endif
}
int LibSoundPlayer::alGetCursor() {
	int offset = 0;
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alGetSourcei(source, AL_SAMPLE_OFFSET, &offset); _CHECK_AL_ERROR(); }
#else
	_LOGOUTSYS("call: alGetCursor");
	SLresult slResult = 0;
	SLmillisecond ms = 0;
	if ( sl_source ) { slResult = SL_GetPosition( sl_ifPlayer, &ms ); _CHECK_SL_ERROR(); }
	// ミリ秒で帰ってくるので、Sample単位に変換
	offset = ms * buf->bitrate() * ( buf->bits() / 8 ) * buf->channel() / 1000;
#endif
	return offset;
}

void LibSoundPlayer::alSetVolume( float vol ) {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alSourcef(source, AL_GAIN, vol); _CHECK_AL_ERROR(); }
#else
	_LOGOUTSYS("call: alSetVolume");
	SLresult slResult = 0;
	// 0.0 - 1.0 からミリベル単位に変換
	SLmillibel millibel;
	millibel = (SLmillibel)( (vol >= 1.0f) ? 0 : ((vol < 0.01f) ? -16000 : (SLmillibel)(8000.0f*log10f(vol))));
	if ( sl_source ) { slResult = SL_SetVolumeLevel( sl_ifVolume, millibel );  _CHECK_SL_ERROR(); }
#endif
}

void LibSoundPlayer::alSetPitch( float pitch ) {
#ifndef	CODE_USE_OPENSL_ES
	if ( source ) { alSourcef(source, AL_PITCH, pitch ); _CHECK_AL_ERROR(); }
#else
	_LOGOUTSYS("call: alSetPitch");
	SLresult slResult = 0;
	// 0.0 - 1.0(def) - max から、1000倍変換
	SLpermille rate = (SLpermille)( pitch * 1000.f );
	// rate範囲を超えないようにする
	if ( rate < sl_rateMin ) { rate = sl_rateMin; }
	if ( rate > sl_rateMax ) { rate = sl_rateMax; }
	if ( sl_source ) { slResult = SL_SetRate( sl_ifRate, rate ); _CHECK_SL_ERROR(); }
#endif
}



#ifdef	CODE_USE_OPENSL_ES

/*
#ifdef	_ANDROID_
#define LOGV_ENABLE 0
#define LOGD_ENABLE 0
#define LOGI_ENABLE 1
#include "../jnientry.h"
#endif
*/

// callback
void LibSoundPlayer::_slCallbackPlayer( SLAndroidSimpleBufferQueueItf, void *context ) { ((LibSoundPlayer*)context)->slCallbackPlayer(); }
void LibSoundPlayer::slCallbackPlayer() {

//	LOGI("slCallbackPlayer()");

	if ( buf->buftype == LibSoundUtil::BUFTYPE_STATIC ) {
		_LOGOUTSYS("slcallback_static: %d", m_loop_cnt);

		if ( m_loop_cnt != 1 ) {
			// ループ処理を行う
			buf->sl_bufcur_read = 0;
			_quepost( 1, NULL );
			alPlay();
		} else {
			alStop();
		}

		// ループまだするか？
		if ( m_loop_cnt != 1 ) {
			// ループカウンタを減らす
			if ( m_loop_cnt > 1 ) { m_loop_cnt--; }	// 0:無限ループの場合はカウンタ減らさない
		}

	} else if ( buf->buftype == LibSoundUtil::BUFTYPE_STREAM ) {
		_quepost( 1, NULL );
		if ( !sl_nextlast ) {
			int processcnt;
			int play_finish = _que( 1, NULL, &processcnt );
			if ( play_finish ) {
				// 次は無い
				sl_nextlast = true;
			}
		} else {
			//stop();
		}
	}
}
#endif

int LibSoundPlayer::_que( int processed, void *quelist, int *pprocesscnt ) {

#ifndef	CODE_USE_OPENSL_ES
	ALuint *unqueue_buffer_id = (ALuint *)quelist;
#endif

	// datarateの計算
	int datarate = buf->bits() / 8 * buf->channel();

	int tmppos = 0, i, play_finish = 0, processcnt = 0;
	for ( i = 0; i < processed; i++ ) {
		// 読み込みをする。サンプル値を計算していき、ループ末端まで行ったらキューを切る
		bool bLoopLast = false;
		int loadsize = LibSoundUtil::STREAM_BUF_SIZE;
		int loopend = ( m_loop_start + m_loop_length ) * datarate;
		tmppos = 0;

		// load_sample_baseを保持しておく
		int j;
		for ( j = 0; j < LibSoundUtil::BUFFER_SIZE - 1; j++ ) {
			m_load_sample_base[j] = m_load_sample_base[j+1];
		}
		m_load_sample_base[j] = m_load_sample / datarate;

		// ループポイントを過ぎて読み込もうとしているか？
		if ( m_loop_cnt != 1 ) {
			if ( m_load_sample + loadsize > loopend ) {
				// ループポイントまでの読み込みとする
				loadsize = loopend - m_load_sample;
				bLoopLast = true;
			}
		}

		// loadsize分読み込む。ちょうど読み込むことはできないので、多めに読み込んでおく
		void *tmp = buf->storage->read( loadsize, &tmppos );
		m_load_sample += tmppos;

		// ファイル最後まで読み込んだか？
		if ( tmppos == 0 ) {
			// 最後まで再生した
			if ( m_loop_cnt != 1 ) {
				bLoopLast = true;
			} else {
				play_finish = 1;
				if ( tmp ) { free( tmp ); }
				break;
			}
		}

		// ループポイント以降で余計に読み込んだか？
		if ( bLoopLast && tmppos > loadsize ) {
			// 余計に読み込んだ分を切り捨てる
			tmppos = loadsize;
		}

		// 波形をバッファに入れる
#ifndef	CODE_USE_OPENSL_ES
		if ( tmppos ) {
			alBufferData( unqueue_buffer_id[i], buf->format, tmp, tmppos, buf->storage->m_pcmdata_rate ); _CHECK_AL_ERROR();
			processcnt++;
		}
		if ( tmp ) { free( tmp ); }
#else
		if ( tmppos ) {
//_LOGOUTSYS("slque, %x, %d", buf->sl_buffers[buf->sl_bufcur_write], buf->sl_bufcur_write);
			if ( buf->sl_buffers[buf->sl_bufcur_write] ) { free( buf->sl_buffers[buf->sl_bufcur_write] ); buf->sl_buffers[buf->sl_bufcur_write] = NULL; }
			buf->sl_buffers[buf->sl_bufcur_write] = tmp;
			buf->sl_buffers_len[buf->sl_bufcur_write] = tmppos;
			buf->sl_bufcur_write = ( buf->sl_bufcur_write + 1 ) % LibSoundUtil::BUFFER_SIZE;
			processcnt++;
		}
		else if ( tmp ) { free( tmp ); }
#endif

		// ループ最後まで行ったか？
		if ( bLoopLast ) {
			//printf( "ReadLast!:%d / %d\n", loadsize, ( loop_start + loop_length ) / 4 );
			// ループ回数は問題無いか？
			if ( m_loop_cnt != 1 ) {
				// ループカウンタを減らす
				if ( m_loop_cnt > 1 ) { m_loop_cnt--; }
				// ループスタートの位置に戻る
				buf->storage->seek( m_loop_start );
				m_load_sample = m_loop_start * datarate;
				// loopポイントではなく、ファイル終端まで行ったことによる終了か？
				if ( tmppos == 0 ) {
					i--; continue;	// 読み直す
				}
			}
		}
	}

	*pprocesscnt = processcnt;

	return play_finish;
}

int LibSoundPlayer::_quepost( int processcnt, void *quelist ) {
#ifndef	CODE_USE_OPENSL_ES
	ALuint *unqueue_buffer_id = (ALuint *)quelist;
#endif
	if ( processcnt > 0 ) {
		// データを埋めたのでキューイング
#ifndef	CODE_USE_OPENSL_ES
		alSourceQueueBuffers( source, processcnt, unqueue_buffer_id ); _CHECK_AL_ERROR();
#else
		// SLはキューイングを１つしかできない
		SL_Enqueue( sl_ifQBuffer, buf->sl_buffers[buf->sl_bufcur_read], buf->sl_buffers_len[buf->sl_bufcur_read] );
		buf->sl_bufcur_read = ( buf->sl_bufcur_read + 1 ) % LibSoundUtil::BUFFER_SIZE;
#endif
	}

	return 1;
}


void LibSoundPlayer::_play_stream_first() {
	// ストレージのオープン(すでに開かれてるはずだけど、close/playされた時のために)
	int pcmdata_rate, pcmdata_bits, pcmdata_channels;
	buf->storage->open( &pcmdata_rate, &pcmdata_bits, &pcmdata_channels );

	// スタート位置に移動
	buf->storage->seek( m_start_cur );
	m_load_sample = m_start_cur * ( buf->bits() / 8 * buf->channel() );

#if 0	// ここにやると、再生のたびにセットしなおすのでダメだあ・・
	{
		OggVorbis_File *povf = (OggVorbis_File *)( &( reinterpret_cast<LibSoundStorage_Ogg*>(buf->storage->pStorageClass)->m_ovf ) );

		// ファイル情報の取得
		vorbis_info *ovi = ov_info( povf, -1 );
		// コメントを検索してループ位置を決める
		for( int i = 0; i < povf->vc->comments; i++ ) {
			char *cc = povf->vc->user_comments[i];
			if ( strncmp( cc, "LOOPSTART=", 10 ) == 0 ) {
				m_loop_start = atoi( &cc[10] );
			} else if ( strncmp( cc, "LOOPLENGTH=", 11 ) == 0 ) {
				m_loop_length = atoi( &cc[11] );
			}
		}
	}
#endif
}

void LibSoundPlayer::_play_stream_finish() {
	// 後処理
	buf->storage->close();
}


#ifndef	CODE_USE_OPENSL_ES
void LibSoundPlayer::_play_thread( void *pParam ) {
	if ( !buf ) {
		m_play_thread_run = false;
		return;
	}

	// stream再生のバッファ監視
	ALuint unqueue_buffer_id[LibSoundUtil::BUFFER_SIZE];
	int play_finish = 0;
	while( play_finish == 0 && !m_play_thread_finish ) {
		LibSoundUtil::SleepThread( 1 );

		int processed;

		if ( m_stream_first ) {
			// 初回処理
			// まだ再生前なので全てのバッファが空いていることにする
			for ( processed = 0; processed < LibSoundUtil::BUFFER_SIZE; processed++ ) {
				unqueue_buffer_id[processed] = buf->buffers[processed];
			}
			_play_stream_first();
		} else {
			// バッファが空いているか確認する
			alGetSourcei( source, AL_BUFFERS_PROCESSED, &processed ); _CHECK_AL_ERROR();
			if ( processed > 0 ) {
				alSourceUnqueueBuffers( source, processed, unqueue_buffer_id ); _CHECK_AL_ERROR();
				//printf( "Buffer Processed: %d, %d\n", processed, GetTickCount() );
			}
		}

		// バッファが空いていたらキューイング
		if ( processed > 0 ) {
			int processcnt;
			play_finish = _que( processed, unqueue_buffer_id, &processcnt );
			if ( processcnt > 0 ) {
				_quepost( processcnt, unqueue_buffer_id );
			}
		}

		// 再生開始
		if ( m_stream_first ) {
			alPlay();
			m_stream_first = false;
		}

	}

	_play_stream_finish();

	m_play_thread_run = false;

	return;
}
#endif


void LibSoundPlayer::onTick() {
	if ( !isplay() && !ispause() ) {
		// 再生が止まった。一時停止中でもない
		selfdead();
	}
}

void LibSoundPlayer::selfdead() {
	if ( m_stopdead ) {
		m_stopdead = false;
		m_libsnd->del( this );
	}
}

void LibSoundPlayer::play() {
	if ( !buf ) { return; }

	if ( !isplay() ) {
		// 再生タイプによっての事前処理
		if ( buf->buftype == LibSoundUtil::BUFTYPE_STATIC ) {
#ifndef	CODE_USE_OPENSL_ES
			// バッファをソースにアタッチ
			alSourcei( source, AL_BUFFER, buf->buffers[0] ); _CHECK_AL_ERROR();
#else
			// キューを設定
			SL_Enqueue( sl_ifQBuffer, buf->sl_buffers[0], buf->sl_buffers_len[0] );
#endif

		} else if ( buf->buftype == LibSoundUtil::BUFTYPE_STREAM ) {
			// ストリーム再生用準備
		}

		// volume set, is mute...
		setvolume();
		setpitch();
		setmute();

		// play.
		if ( buf->buftype == LibSoundUtil::BUFTYPE_STATIC ) {
			alSetCursor( m_start_cur );
			alPlay();

		} else if ( buf->buftype == LibSoundUtil::BUFTYPE_STREAM ) {

#ifndef	CODE_USE_OPENSL_ES
			if ( m_play_thread_run ) {
				// すでにマルチスレッド起動済(pause?)
				alPlay();
			} else {
				// マルチスレッド起動
				m_stream_first = true;
				m_play_thread_run = true;
				m_play_thread_finish = false;
				THREADCALL_PLAY_THREAD();
			}
#else
			if ( sl_play_run ) {
				// すでに再生中(pause?)
				alPlay();

			} else {
				sl_play_run = true;

				int processcnt;
				sl_nextlast = false;
				_play_stream_first();
				int play_finish = _que( buf->buffers_num, NULL, &processcnt );
				if ( processcnt > 0 ) {
					_quepost( 1, NULL );
				}
				if ( play_finish ) {
					sl_nextlast = true;
				}
				alPlay();
			}
#endif
		}
	}

}
void LibSoundPlayer::stop() { stop( false, false ); }
void LibSoundPlayer::stop( bool nodead, bool destract ) {
	if ( !buf ) { return; }

	alStop();
	m_start_cur = 0;

	if ( buf->buftype == LibSoundUtil::BUFTYPE_STREAM ) {
#ifndef	CODE_USE_OPENSL_ES
		if ( m_play_thread_run ) {
			int cnt = 0;
			m_play_thread_finish = true;
			while( m_play_thread_run ) {
				// 止まるまでwait
				//printf( "... thead(1) dead wait. %d\n", cnt++ );
				LibSoundUtil::SleepThread( 1 );
			}
			THREADSTOP_PLAY_THREAD();
		}
#else
		_play_stream_finish();
		sl_play_run = false;
#endif

		if ( !destract ) {
			// player作り直し
			alDeletePlayer();
			alCreatePlayer();
		}
	}

	if ( !nodead && !destract ) { selfdead(); }
}

void LibSoundPlayer::pause() {
	alPause();
}

bool LibSoundPlayer::ispause() {
	return alIsPause();
}


bool LibSoundPlayer::isplay() {
	if ( alIsPlay() ) { return true; }
	if ( m_stream_first ) {	// ストリーム再生指示開始している場合
		return true;
	}
	return false;
}


int LibSoundPlayer::loopcnt() {
	return m_loop_cnt;
}
void LibSoundPlayer::setloop( int loopcnt ) {
	if ( !buf ) { return; }

	m_loop_cnt = loopcnt;
	if ( buf->buftype == LibSoundUtil::BUFTYPE_STATIC ) {
#ifndef	CODE_USE_OPENSL_ES
		if ( m_loop_cnt != 1 ) {
			alSourcei(source,AL_LOOPING,AL_TRUE); _CHECK_AL_ERROR();
		} else {
			alSourcei(source,AL_LOOPING,AL_FALSE); _CHECK_AL_ERROR();
		}
#else
		// todo:setloopしなきゃ・・callbackで処理してるからいらないか
#endif
	}
}

int LibSoundPlayer::loopstart() {
	return m_loop_start;
}
void LibSoundPlayer::setloopstart( int start ) {
	m_loop_start = start;
}
int LibSoundPlayer::looplength() {
	return m_loop_length;
}
void LibSoundPlayer::setlooplength( int length ) {
	m_loop_length = length;
}



// def:100
int LibSoundPlayer::pitch() {
	return m_pitch;
}

void LibSoundPlayer::setpitch( int pitch ) {
	if ( pitch < 1 ) { pitch = 1; }
	m_pitch = pitch;
	setpitch();
}
void LibSoundPlayer::setpitch() {
	alSetPitch( (float)( m_pitch * m_libsnd->pitch() ) / 10000.f );
}

// 0 - 100
int LibSoundPlayer::volume() {
	return m_vol;
}

void LibSoundPlayer::setvolume( int vol ) {
	if ( vol < 0 ) { vol = 0; }
	if ( vol > 100 ) { vol = 100; }
	m_vol = vol;
	setvolume();
}
void LibSoundPlayer::setvolume() {
	if ( !ismute() ) {
		alSetVolume( (float)( m_vol * m_libsnd->volume() ) / 10000.f );
	}
}

void LibSoundPlayer::setmute( bool mute ) {
	m_mute = mute;
	setmute();
}
void LibSoundPlayer::setmute() {
	if ( m_libsnd->ismute() || m_mute ) {
		alSetVolume( 0 );
	} else {
		setvolume( m_vol );
	}
}

bool LibSoundPlayer::ismute() {
	if ( m_libsnd->ismute() ) { return true; }
	return m_mute;
}

bool LibSoundPlayer::stopdead() {
	return m_stopdead;
}
void LibSoundPlayer::setstopdead( bool dead ) {
	m_stopdead = dead;
}


int LibSoundPlayer::cursor() {
	if ( !buf ) { return 0; }

	int offset = 0;
	if ( isplay() || ispause() ) {
		if ( buf->buftype == LibSoundUtil::BUFTYPE_STATIC ) {
			offset = alGetCursor();
		} else if ( buf->buftype == LibSoundUtil::BUFTYPE_STREAM ) {
			offset = alGetCursor() + m_load_sample_base[0];
		}
	}

	return offset;
}

void LibSoundPlayer::setcursor( int cursor ) {
	int fplay = isplay();
	if ( fplay ) { stop(true,false); }
	if ( cursor < 0 ) { cursor = 0; }
	m_start_cur = cursor;
	if ( fplay ) { play(); }
}
