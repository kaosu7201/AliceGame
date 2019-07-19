#ifndef	__LIBSOUNDBUFFER_H__
#define	__LIBSOUNDBUFFER_H__

class LibSound;

class LibSoundBuffer
{
public:
	LibSound	*m_libsnd;
	int			attach_player_cnt;
	bool		delete_reserve;

#ifndef	CODE_USE_OPENSL_ES
	ALuint		buffers[LibSoundUtil::BUFFER_SIZE];
#else
	void		*sl_buffers[LibSoundUtil::BUFFER_SIZE];
	int			sl_buffers_len[LibSoundUtil::BUFFER_SIZE];
	int			sl_bufcur_read, sl_bufcur_write;
#endif
	int			buffers_num;

	int			buffers_rate;
	LibSoundUtil::BUFTYPE		buftype;		// static or stream
	LibSoundUtil::BUFFERFORMAT	format;			// 8-mono, 8-stereo, 16-mono, 16-stereo

	bool		m_file_loop;
	int			m_file_loopstart;
	int			m_file_looplength;

	LibSoundBuffer(LibSound*);
	~LibSoundBuffer();

	int bitrate();	// ex: 44100
	int bits();		// 8 or 16
	int channel();	// 1 or 2

	LibSoundStorage	*storage;
	void		*stream_mem_buffer;	// streaméûÇ…ÉÅÉÇÉäÇ©ÇÁì«Ç›çûÇﬁóp
};




#endif
