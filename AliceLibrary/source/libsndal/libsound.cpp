/*
** openal, libsound.
*/

#include "libsound.h"




LibSound::LibSound() {

	// パラメータの初期化
	m_pitch = 100;
	m_vol = 100;
	m_mute = false;

#ifndef	CODE_USE_OPENSL_ES
	//デバイスを開く
	device = alcOpenDevice(NULL); _CHECK_AL_ERROR();
	
	//コンテキストを生成
	context = alcCreateContext(device, NULL); _CHECK_AL_ERROR();

	//使用するコンテキストの指定
	alcMakeContextCurrent(context); _CHECK_AL_ERROR();
#else
	SLresult slResult = 0;

	// エンジンオブジェクトとインタフェイス
	slResult = slCreateEngine( &engineObject, 0, NULL, 0, NULL, NULL ); _CHECK_SL_ERROR();
	slResult = SL_Realize( engineObject, SL_BOOLEAN_FALSE ); _CHECK_SL_ERROR();
	slResult = SL_GetInterface( engineObject, SL_IID_ENGINE, &engineIf ); _CHECK_SL_ERROR();

	// ミキサ
	slResult = SL_CreateOutputMix(engineIf, &mixerObject, 0, NULL, NULL); _CHECK_SL_ERROR();
	slResult = SL_Realize(mixerObject, SL_BOOLEAN_FALSE); _CHECK_SL_ERROR();

#endif

}

LibSound::~LibSound() {

	// 管理してるPlayer,Bufferを削除
	{	// player
		list<LibSoundPlayer*> l = list_player;	// 中で消えるからコピーしないとダメ
		list<LibSoundPlayer*>::iterator it = l.begin();
		while( it != l.end() ) { del( *it ); ++it; }
	}
	{	// buffer
		list<LibSoundBuffer*> l = list_buffer;	// 中で消えるからコピーしないとダメ
		list<LibSoundBuffer*>::iterator it = l.begin();
		while( it != l.end() ) { del( *it ); ++it; }
	}

#ifndef	CODE_USE_OPENSL_ES
	alcMakeContextCurrent(NULL); _CHECK_AL_ERROR();
	alcDestroyContext(context); _CHECK_AL_ERROR();
	alcCloseDevice(device); _CHECK_AL_ERROR();
#else
	SL_Destroy( mixerObject );
	SL_Destroy( engineObject );
#endif
}

// selfdeadなど監視を必要とする機能を実装する場合は一定間隔でこれを呼び出す必要がある
void LibSound::onTick() {
	// 管理してるPlayerのonTickを呼び出す
	{	// player
		list<LibSoundPlayer*> l = list_player;	// 中で何が起こるか不定なのでコピーしてから
		list<LibSoundPlayer*>::iterator it = l.begin();
		while( it != l.end() ) {
			reinterpret_cast<LibSoundPlayer*>(*it)->onTick();
			++it;
		}
	}
}




LibSoundBuffer *LibSound::attach( char *szFile ) {
	return _attach( LibSoundUtil::STORAGETYPE_FILE, (void*)szFile, 0 );

}
LibSoundBuffer *LibSound::attach( void *pData, int nDataSize ) {
	// streamをメモリから読み込む時は、コピーしたものを使用する
	void *p = malloc( nDataSize );
	memcpy( p, pData, nDataSize );
	LibSoundBuffer *buf = _attach( LibSoundUtil::STORAGETYPE_MEMORY, p, nDataSize );
	if ( buf ) {
		buf->stream_mem_buffer = p;
	}
	return buf;
}


LibSoundStorage *LibSound::_storageopen( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize, int *pcmdata_rate, int *pcmdata_bits, int *pcmdata_channels, LibSoundUtil::BUFFERFORMAT *format ) {
	LibSoundStorage *storage = new LibSoundStorage( storagetype, pData, nDataSize );
	if ( !storage->open( pcmdata_rate, pcmdata_bits, pcmdata_channels ) ) { return NULL; }

	// 帰ってきたデータから、ALFORMATを選択
	if ( *pcmdata_bits ==  8 && *pcmdata_channels == 1 ) { *format = LibSoundUtil::BUFFERFORMAT_MONO8; }
	if ( *pcmdata_bits ==  8 && *pcmdata_channels == 2 ) { *format = LibSoundUtil::BUFFERFORMAT_STEREO8; }
	if ( *pcmdata_bits == 16 && *pcmdata_channels == 1 ) { *format = LibSoundUtil::BUFFERFORMAT_MONO16; }
	if ( *pcmdata_bits == 16 && *pcmdata_channels == 2 ) { *format = LibSoundUtil::BUFFERFORMAT_STEREO16; }

	return storage;
}

LibSoundBuffer *LibSound::_attach( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize ) {

	// データをストレージに入れて処理する(まだ開くだけ）
	int pcmdata_len = 0, pcmdata_rate, pcmdata_bits, pcmdata_channels;
	LibSoundUtil::BUFFERFORMAT format;
	LibSoundStorage *storage = _storageopen( storagetype, pData, nDataSize, &pcmdata_rate, &pcmdata_bits, &pcmdata_channels, &format );
	if ( !storage ) { return NULL; }

	// ファイルのループ情報を得る
	bool fileloop;
	int fileloopstart, filelooplength;
	fileloop = storage->getfileloopsetting( &fileloopstart, &filelooplength );

	// バッファを4つ作る（リングバッファ用）
	LibSoundBuffer *buf = NULL;
	buf = new LibSoundBuffer(this);
	add( buf );
	buf->buffers_num = LibSoundUtil::BUFFER_SIZE;

	// バッファの生成
#ifndef	CODE_USE_OPENSL_ES
	alGenBuffers( buf->buffers_num, buf->buffers ); _CHECK_AL_ERROR();
#else
	for ( int i = 0; i < buf->buffers_num; i++ ) { buf->sl_buffers[i] = NULL; }
#endif
	buf->storage = storage;
	buf->buffers_rate = pcmdata_rate;
	buf->format = format;

	// ループ情報の記録
	buf->m_file_loop = fileloop;
	buf->m_file_loopstart	= fileloopstart;
	buf->m_file_looplength	= filelooplength;

	// タイプの記録
	buf->buftype = LibSoundUtil::BUFTYPE_STREAM;

	return buf;
}

LibSoundBuffer *LibSound::load( char *szFile ) {
	return _load( LibSoundUtil::STORAGETYPE_FILE, (void*)szFile, 0 );
}

LibSoundBuffer *LibSound::load( void *pData, int nDataSize ) {
	return _load( LibSoundUtil::STORAGETYPE_MEMORY, (void*)pData, nDataSize );
}


LibSoundBuffer *LibSound::_load( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize ) {

	// データをストレージに入れて処理する(読み込みbytesを-1にして、全データをPCM化)
	int pcmdata_len = 0, pcmdata_rate, pcmdata_bits, pcmdata_channels;
	LibSoundUtil::BUFFERFORMAT format;
	LibSoundStorage *storage = _storageopen( storagetype, pData, nDataSize, &pcmdata_rate, &pcmdata_bits, &pcmdata_channels, &format );
	if ( !storage ) { return NULL; }
	void *pcmdata = storage->read( -1, &pcmdata_len );

	// ファイルのループ情報を得る
	bool fileloop;
	int fileloopstart, filelooplength;
	fileloop = storage->getfileloopsetting( &fileloopstart, &filelooplength );

	storage->close();

	// バッファを作成して、そこにデータを入れる
	LibSoundBuffer *buf = NULL;

	if ( pcmdata ) {
		// バッファを作成して、そこにデータを入れる
		buf = new LibSoundBuffer(this);
		add( buf );
		buf->buffers_num = 1;

#ifndef	CODE_USE_OPENSL_ES
		// バッファの生成
		alGenBuffers( buf->buffers_num, buf->buffers ); _CHECK_AL_ERROR();

		// 波形をバッファに入れる
		alBufferData( buf->buffers[0], (ALenum)format, pcmdata, pcmdata_len, pcmdata_rate ); _CHECK_AL_ERROR();

		// 波形データを解放
		free( pcmdata );
#else
		// SLのバッファは単なるメモリ。そのままpcmdataを保持
		buf->sl_buffers[0]		= pcmdata;
		buf->sl_buffers_len[0]	= pcmdata_len;
#endif
		buf->buffers_rate = pcmdata_rate;
		buf->format = format;
		buf->storage = storage;

		// ループ情報の記録
		buf->m_file_loop = fileloop;
		buf->m_file_loopstart	= fileloopstart;
		buf->m_file_looplength	= filelooplength;

		// タイプの記録
		buf->buftype = LibSoundUtil::BUFTYPE_STATIC;

	}

	return buf;
}



//#define	DBG_TIME
#ifdef	DBG_TIME
#ifdef	_ANDROID_
#define LOGV_ENABLE 0
#define LOGD_ENABLE 0
#define LOGI_ENABLE 1
#include "../jnientry.h"
#endif
#endif

LibSoundPlayer *LibSound::player( LibSoundBuffer *buf ) {

#ifdef	DBG_TIME
	long nb = LibSoundUtil::getTickCount();
#endif

	LibSoundPlayer *pl = new LibSoundPlayer(this);

#ifdef	DBG_TIME
	long nb_1 = LibSoundUtil::getTickCount() - nb;
#endif

	add( pl );

#ifdef	DBG_TIME
	long nb_2 = LibSoundUtil::getTickCount() - nb;
#endif

	// バッファのアタッチカウントを+1
	buf->attach_player_cnt++;

	// プレイヤーにバッファを保持させる
	pl->buf = buf;

	// ソースを生成
	pl->alCreatePlayer();

#ifdef	DBG_TIME
	long nb_3 = LibSoundUtil::getTickCount() - nb;
#endif

	// ファイルのループポイントをセット
	if ( buf->m_file_loop ) {
		pl->setloopstart( buf->m_file_loopstart );
		pl->setlooplength( buf->m_file_looplength );
	}

#ifdef	DBG_TIME
	long nb_4 = LibSoundUtil::getTickCount() - nb;
	LOGI("sndPlayer: %d %d %d %d", nb_1, nb_2, nb_3, nb_4 );
#endif

	return pl;
}

void LibSound::add( LibSoundPlayer *pl ) {
	list_player.push_back( pl );
}
void LibSound::add( LibSoundBuffer *buf ) {
	list_buffer.push_back( buf );
}

void LibSound::del( LibSoundPlayer *pl ) {
	list<LibSoundPlayer*>::iterator cnt = std::find( list_player.begin(), list_player.end(), pl );
	if ( cnt != list_player.end() && list_player.begin() != list_player.end() ) {
		pl->buf->attach_player_cnt--;
		LibSoundBuffer *buf = NULL;
		if ( pl->buf->delete_reserve && pl->buf->attach_player_cnt == 0 ) {
			// 削除予約フラグが入っていて、アタッチカウンタが0になった。削除
			buf = pl->buf;
		}
		list_player.remove( pl );
		delete pl;
		if ( buf ) {
			del( buf );
		}
	}
}
void LibSound::del( LibSoundBuffer *buf ) {
	list<LibSoundBuffer*>::iterator cnt = std::find( list_buffer.begin(), list_buffer.end(), buf );
	if ( cnt != list_buffer.end() && list_buffer.begin() != list_buffer.end() ) {
		if ( buf->attach_player_cnt == 0 ) {
			list_buffer.remove( buf );
			if ( buf->storage ) { delete buf->storage; }
			if ( buf->stream_mem_buffer ) { free( buf->stream_mem_buffer ); }

#ifndef	CODE_USE_OPENSL_ES
			alDeleteBuffers( buf->buffers_num, buf->buffers); _CHECK_AL_ERROR();
#else
			for ( int i = 0; i < LibSoundUtil::BUFFER_SIZE; i++ ) {
				if ( buf->sl_buffers[i] ) {
					free( buf->sl_buffers[i] );
					buf->sl_buffers[i] = NULL;
				}
			}
#endif
			delete buf;
		} else {
			// まだplayerが居る。削除予約フラグを付けておく
			buf->delete_reserve = true;
		}
	}
}


// def:100
int LibSound::pitch() {
	return m_pitch;
}

void LibSound::setpitch( int pitch ) {
	if ( pitch < 1 ) { pitch = 1; }
	m_pitch = pitch;
	{	// player
		list<LibSoundPlayer*> l = list_player;	// 中で何が起こるか不定なのでコピーしてから
		list<LibSoundPlayer*>::iterator it = l.begin();
		while( it != l.end() ) {
			reinterpret_cast<LibSoundPlayer*>(*it)->setpitch();
			++it;
		}
	}
}

// 0 - 100
int LibSound::volume() {
	return m_vol;
}

void LibSound::setvolume( int vol ) {
	if ( vol < 0 ) { vol = 0; }
	if ( vol > 100 ) { vol = 100; }
	m_vol = vol;
	if ( !ismute() ) {
		{	// player
			list<LibSoundPlayer*> l = list_player;	// 中で何が起こるか不定なのでコピーしてから
			list<LibSoundPlayer*>::iterator it = l.begin();
			while( it != l.end() ) {
				reinterpret_cast<LibSoundPlayer*>(*it)->setvolume();
				++it;
			}
		}
	}
}

// mute
void LibSound::setmute( bool mute ) {
	m_mute = mute;
	{	// player
		list<LibSoundPlayer*> l = list_player;	// 中で何が起こるか不定なのでコピーしてから
		list<LibSoundPlayer*>::iterator it = l.begin();
		while( it != l.end() ) {
			reinterpret_cast<LibSoundPlayer*>(*it)->setmute();
			++it;
		}
	}

}

bool LibSound::ismute() {
	return m_mute;
}


// pause
void LibSound::setpause( bool pause ) {
	m_pause = pause;
	{	// player
		list<LibSoundPlayer*> l = list_player;	// 中で何が起こるか不定なのでコピーしてから
		list<LibSoundPlayer*>::iterator it = l.begin();
		while( it != l.end() ) {
			if ( m_pause ) {
				if ( reinterpret_cast<LibSoundPlayer*>(*it)->isplay() ) {
					reinterpret_cast<LibSoundPlayer*>(*it)->pause();
				}
			} else {
				if ( reinterpret_cast<LibSoundPlayer*>(*it)->ispause() ) {
					reinterpret_cast<LibSoundPlayer*>(*it)->play();
				}

			}
			++it;
		}
	}

}

bool LibSound::ispause() {
	return m_pause;
}
