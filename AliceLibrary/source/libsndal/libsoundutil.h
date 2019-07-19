#ifndef	__LIBSOUNDUTIL_H__
#define	__LIBSOUNDUTIL_H__

#ifndef	CODE_USE_OPENSL_ES
	#ifndef	_DEBUG
		#define	_CHECK_AL_ERROR()
	#else
		#define	_CHECK_AL_ERROR()	LibSoundUtil::_check_al_error(__FILE__,__LINE__)
	#endif
#else
//	#ifndef	_DEBUG
//		#define	_CHECK_SL_ERROR()
//	#else
		#define	_CHECK_SL_ERROR()	LibSoundUtil::_check_sl_error(slResult,__FILE__,__LINE__)
//	#endif

#endif


class LibSoundUtil
{
public:
	enum BUFTYPE {
		BUFTYPE_STATIC = 0,
		BUFTYPE_STREAM,
	};
	enum DATATYPE {
		DATATYPE_ERR = 0,
		DATATYPE_PCM,
		DATATYPE_WAV,
		DATATYPE_OGG,
	};
	enum STORAGETYPE {
		STORAGETYPE_MEMORY = 0,
		STORAGETYPE_FILE,
	};

#ifndef	CODE_USE_OPENSL_ES
	enum BUFFERFORMAT {
		BUFFERFORMAT_NONE = 0,
		BUFFERFORMAT_MONO8		= AL_FORMAT_MONO8,
		BUFFERFORMAT_STEREO8	= AL_FORMAT_STEREO8,
		BUFFERFORMAT_MONO16		= AL_FORMAT_MONO16,
		BUFFERFORMAT_STEREO16	= AL_FORMAT_STEREO16,
	};
#else
	enum BUFFERFORMAT {
		BUFFERFORMAT_NONE = 0,
		BUFFERFORMAT_MONO8,
		BUFFERFORMAT_STEREO8,
		BUFFERFORMAT_MONO16,
		BUFFERFORMAT_STEREO16,
	};
#endif
	enum {
#ifndef	CODE_USE_OPENSL_ES
		BUFFER_SIZE = 4,						// ストリーム用リングバッファ個数
		STREAM_BUF_SIZE = (44100*(16/8)*2*1),	// ストリーム用リングバッファサイズ 44.100KHz, 16bit, Stereo, 1sec.
#else
		BUFFER_SIZE = 2,						// ストリーム用リングバッファ個数, SLは複数キューイングできないので2で固定
//		STREAM_BUF_SIZE = (44100*(16/8)*2*3/10),	// ストリーム用リングバッファサイズ 44.100KHz, 16bit, Stereo, 0.3sec.
//		STREAM_BUF_SIZE = (44100*(16/8)*2*5),	// ストリーム用リングバッファサイズ 44.100KHz, 16bit, Stereo, 5sec.(バッファ大きいとスキマが酷い)
//		STREAM_BUF_SIZE = (44100*(16/8)*2*1/10),	// ストリーム用リングバッファサイズ 44.100KHz, 16bit, Stereo, 0.1sec.
		STREAM_BUF_SIZE = (44100*(16/8)*2*1/10/4),	// ストリーム用リングバッファサイズ 44.100KHz, 16bit, Stereo, 0.1/4sec.(多分最小)
//		STREAM_BUF_SIZE = (44100*(16/8)*2*1/10/8),	// ストリーム用リングバッファサイズ 44.100KHz, 16bit, Stereo, 0.1/8sec.(さらに最小)
#endif
	};



#ifndef	CODE_USE_OPENSL_ES
	static int _check_al_error( const char *szFile, int nLine );
#else
	static int _check_sl_error( SLresult slResult, const char *szFile, int nLine );
#endif

	static void SleepThread( unsigned int ms );
	static long getTickCount();


#ifndef	_ANDROID_
	typedef	FILE	FILEDATA;
#else
	typedef	struct {
		AAsset*		asset_fp;
		FILE*		fp;
		long		cur;
		long		size;
	} FILEDATA;
private:
	static AAssetManager	*assetManager;
public:
#endif
	static void		fileinit( void* );

	static LibSoundUtil::FILEDATA*	fileopen( char *szFile );
	static int		fileclose( LibSoundUtil::FILEDATA* fdata );
	static int		fileread( void *buf, int size, int n, LibSoundUtil::FILEDATA *fdata );
	static int		fileseek( LibSoundUtil::FILEDATA *fdata, long offset, int origin );
	static long		filetell( LibSoundUtil::FILEDATA *fdata );


};


#endif


