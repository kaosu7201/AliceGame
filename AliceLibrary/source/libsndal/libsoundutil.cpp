/*
** Sound系ユーティリティ
** 各プラットホーム上での機能の隠蔽など
*/


#include "libsound.h"

/*
#ifdef	_ANDROID_
#define LOGV_ENABLE 0
#define LOGD_ENABLE 0
#define LOGI_ENABLE 0
#include "../jnientry.h"
#endif
*/

#ifndef	CODE_USE_OPENSL_ES
int LibSoundUtil::_check_al_error( const char *szFile, int nLine ) {
	ALenum error = alGetError();
	if ( error != AL_NO_ERROR ) {
		_LOGOUTSYS( "%s : %d : ", szFile, nLine );
		switch( error ) {
			//case AL_NO_ERROR:		{ printf( "NO_ERROR\n" ); break; }
			case AL_INVALID_NAME:		{ _LOGOUTSYS( "不正なネームの引数です。\n" ); break; }
			case AL_INVALID_ENUM:		{ _LOGOUTSYS( "不正な列挙値の引数です。\n" ); break; }
			case AL_INVALID_VALUE:		{ _LOGOUTSYS( "不正な引数です。\n" ); break; }
			case AL_INVALID_OPERATION:	{ _LOGOUTSYS( "禁止されている呼び出しです。\n" ); break; }
			case AL_OUT_OF_MEMORY:		{ _LOGOUTSYS( "メモリを割り当てる事が出来ません。\n" ); break; }
		}
	}
	return error;
}
#else
int LibSoundUtil::_check_sl_error( SLresult slResult, const char *szFile, int nLine ) {
	if ( slResult != SL_RESULT_SUCCESS ) {
		_LOGOUTSYS( "opensl: %s, %d ", szFile, nLine );
		switch( slResult ) {
			case SL_RESULT_PRECONDITIONS_VIOLATED:	{ _LOGOUTSYS( "err:SL_RESULT_PRECONDITIONS_VIOLATED\n"); break; }
			case SL_RESULT_PARAMETER_INVALID:		{ _LOGOUTSYS( "err:SL_RESULT_PARAMETER_INVALID\n"); break; }
			case SL_RESULT_MEMORY_FAILURE:			{ _LOGOUTSYS( "err:SL_RESULT_MEMORY_FAILURE\n"); break; }
			case SL_RESULT_RESOURCE_ERROR:			{ _LOGOUTSYS( "err:SL_RESULT_RESOURCE_ERROR\n"); break; }
			case SL_RESULT_RESOURCE_LOST:			{ _LOGOUTSYS( "err:SL_RESULT_RESOURCE_LOST\n"); break; }
			case SL_RESULT_IO_ERROR:				{ _LOGOUTSYS( "err:SL_RESULT_IO_ERROR\n"); break; }
			case SL_RESULT_BUFFER_INSUFFICIENT:		{ _LOGOUTSYS( "err:SL_RESULT_BUFFER_INSUFFICIENT\n"); break; }
			case SL_RESULT_CONTENT_CORRUPTED:		{ _LOGOUTSYS( "err:SL_RESULT_CONTENT_CORRUPTED\n"); break; }
			case SL_RESULT_CONTENT_UNSUPPORTED:		{ _LOGOUTSYS( "err:SL_RESULT_CONTENT_UNSUPPORTED\n"); break; }
			case SL_RESULT_CONTENT_NOT_FOUND:		{ _LOGOUTSYS( "err:SL_RESULT_CONTENT_NOT_FOUND\n"); break; }
			case SL_RESULT_PERMISSION_DENIED:		{ _LOGOUTSYS( "err:SL_RESULT_PERMISSION_DENIED\n"); break; }
			case SL_RESULT_FEATURE_UNSUPPORTED:		{ _LOGOUTSYS( "err:SL_RESULT_FEATURE_UNSUPPORTED\n"); break; }
			case SL_RESULT_INTERNAL_ERROR:			{ _LOGOUTSYS( "err:SL_RESULT_INTERNAL_ERROR\n"); break; }
			case SL_RESULT_UNKNOWN_ERROR:			{ _LOGOUTSYS( "err:SL_RESULT_UNKNOWN_ERROR\n"); break; }
			case SL_RESULT_OPERATION_ABORTED:		{ _LOGOUTSYS( "err:SL_RESULT_OPERATION_ABORTED\n"); break; }
			case SL_RESULT_CONTROL_LOST:			{ _LOGOUTSYS( "err:SL_RESULT_CONTROL_LOST\n"); break; }
		}

	}

	return 0;
}
#endif


void LibSoundUtil::SleepThread( unsigned int ms ) {
#ifdef	WIN32
	::Sleep( ms );
#endif
#ifdef	TARGET_OS_IPHONE
	usleep( ms * 1000 );
#endif
#ifdef	_ANDROID_
	usleep( ms * 1000 );
#endif
}

long LibSoundUtil::getTickCount() {
#ifdef	WIN32
	return timeGetTime();
#endif
#ifdef	TARGET_OS_IPHONE
struct timeval tp;
	void *tzp;
	tzp = NULL;
	gettimeofday(&tp,tzp);
	return (unsigned long)( tp.tv_sec*1000 + tp.tv_usec/1000 );
#endif
#ifdef	_ANDROID_
	struct timeval tp;
	struct timezone *tzp = NULL;
	gettimeofday( &tp, tzp );
	return (unsigned long)( tp.tv_sec*1000 + tp.tv_usec/1000 );
#endif
}



// 静的メンバ変数の実体
#ifndef	_ANDROID_
#else
	AAssetManager	*LibSoundUtil::assetManager;
#endif

void LibSoundUtil::fileinit( void* p ) {
#ifndef	_ANDROID_
#else
	assetManager = (AAssetManager*)p;
#endif
}

LibSoundUtil::FILEDATA* LibSoundUtil::fileopen( char *szFile ) {
#ifndef	_ANDROID_
	FILE *fdata = ::fopen( szFile, "rb" );
	return fdata;
#else
	//LOGI("start");
	//_LOGOUTSYS("util-fopen-1");
	FILEDATA *fdata = (FILEDATA *)malloc( sizeof( FILEDATA ) );
	memset( fdata, 0, sizeof( FILEDATA ) );
	if ( assetManager ) {
		fdata->asset_fp = AAssetManager_open(assetManager, szFile, AASSET_MODE_RANDOM);
		if ( !fdata->asset_fp ) {
			free( fdata );
			return NULL;
		}
		fdata->cur = 0;
		fdata->size = AAsset_getLength( fdata->asset_fp );
	} else {
		fdata->fp = ::fopen( szFile, "rb" );
		if ( !fdata->fp ) {
			free( fdata );
			return NULL;
		}
		fdata->cur = 0;
		fseek( fdata->fp, 0, SEEK_END ); fdata->size = ftell( fdata->fp ); fseek( fdata->fp, 0, SEEK_SET );
	}
	//_LOGOUTSYS("util-fopen-fin");
	//LOGI("end");
	return fdata;
#endif
}

int LibSoundUtil::fileclose( LibSoundUtil::FILEDATA* fdata ) {
#ifndef	_ANDROID_
	return fclose( fdata );
#else
	//LOGI("start");
	//_LOGOUTSYS("util-fclose-1");
	if ( fdata->asset_fp ) {
		AAsset_close( fdata->asset_fp );
		fdata->asset_fp = NULL;
	}
	if ( fdata->fp ) {
		fclose( fdata->fp );
		fdata->fp = NULL;
	}
	free( fdata );
	//_LOGOUTSYS("util-fclose-fin");
	//LOGI("end");
	return 0;	// 成功
#endif
}
int	LibSoundUtil::fileread( void *buf, int size, int n, LibSoundUtil::FILEDATA *fdata ) {
#ifndef	_ANDROID_
	return fread( buf, size, n, fdata );
#else
	//LOGI("start");
	//_LOGOUTSYS("util-fread-1");
	int cnt;
	if ( fdata->asset_fp ) {
		cnt = AAsset_read( fdata->asset_fp, buf, size * n );
	} else if ( fdata->fp ) {
		cnt = fread( buf, 1, size*n, fdata->fp );
	}
	fdata->cur += cnt;
	//_LOGOUTSYS("util-fread-fin");
	//LOGI("end");
	return ( cnt / size );
#endif
}
int	LibSoundUtil::fileseek( LibSoundUtil::FILEDATA *fdata, long offset, int origin ) {
#ifndef	_ANDROID_
	return fseek( fdata, offset, origin );
#else
	//LOGI("start");
	//_LOGOUTSYS("util-fseek-1");
	switch( origin ) {
		case SEEK_CUR: { fdata->cur += (int)offset; break; }
		case SEEK_END: { fdata->cur = fdata->size + (int)offset; break; }
		case SEEK_SET: { fdata->cur = (int)offset; break; }
		default:
			return -1;
	}
	int err = 0;
	if ( fdata->cur > fdata->size ) { fdata->cur = fdata->size; err = -1; }
	if ( fdata->cur < 0 ) { fdata->cur = 0; err = -1; }
	if ( fdata->asset_fp ) {
		AAsset_seek( fdata->asset_fp, fdata->cur, SEEK_SET );
	} else if ( fdata->fp ) {
		fseek( fdata->fp, fdata->cur, SEEK_SET );
	}
	//_LOGOUTSYS("util-fseek-fin");
	//LOGI("end");
	return err;
#endif
}
long LibSoundUtil::filetell( LibSoundUtil::FILEDATA *fdata ) {
#ifndef	_ANDROID_
	return ftell( fdata );
#else
	//_LOGOUTSYS("util-ftell");
	return fdata->cur;
#endif
}

