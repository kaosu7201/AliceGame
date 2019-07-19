
#include "libsound.h"


LibSoundBuffer::LibSoundBuffer(LibSound* libsnd) {
	m_libsnd = libsnd;
	attach_player_cnt = 0;
	buffers_num = 0;
	delete_reserve = false;
	storage = NULL;
	stream_mem_buffer = NULL;
	buffers_rate = 0;
#ifndef	CODE_USE_OPENSL_ES

#else
	for ( int i = 0; i < LibSoundUtil::BUFFER_SIZE; i++ ) {
		sl_buffers[i]		= NULL;
		sl_buffers_len[i]	= 0;
	}
	sl_bufcur_read = 0;
	sl_bufcur_write = 0;
#endif


}
LibSoundBuffer::~LibSoundBuffer() {
	m_libsnd->del( this );
}

int LibSoundBuffer::bitrate() {
	return buffers_rate;
}

// 8 or 16
int LibSoundBuffer::bits() {
	if ( format == LibSoundUtil::BUFFERFORMAT_MONO8 )		{ return 8; }
	if ( format == LibSoundUtil::BUFFERFORMAT_MONO16 )		{ return 16; }
	if ( format == LibSoundUtil::BUFFERFORMAT_STEREO8 )		{ return 8; }
	if ( format == LibSoundUtil::BUFFERFORMAT_STEREO16 )	{ return 16; }
	return 0;
}

// 1 or 2
int LibSoundBuffer::channel() {
	if ( format == LibSoundUtil::BUFFERFORMAT_MONO8 )		{ return 1; }
	if ( format == LibSoundUtil::BUFFERFORMAT_MONO16 )		{ return 1; }
	if ( format == LibSoundUtil::BUFFERFORMAT_STEREO8 )		{ return 2; }
	if ( format == LibSoundUtil::BUFFERFORMAT_STEREO16 )	{ return 2; }
	return 0;
}

