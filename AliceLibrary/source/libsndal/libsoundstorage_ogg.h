#ifndef	__LIBSOUNDSTORAGE_OGG_H__
#define	__LIBSOUNDSTORAGE_OGG_H__



// ogg vorbis
#include	<vorbis/vorbisfile.h>

class LibSoundStorage;

class LibSoundStorage_Ogg {

public:
	LibSoundStorage_Ogg( LibSoundStorage *storage );
	~LibSoundStorage_Ogg();

	static bool checkdatatype( char *p );

	int open( int *pcmrate, int *pcmbits, int *pcmchannels );
	void close();
	void *read( int needbytes, int *readbytes );
	void seek( unsigned int pos );

	bool getfileloopsetting( int *st, int *len );

private:
	LibSoundStorage *m_storage;





public:
// ogg..
	OggVorbis_File m_ovf;
	bool m_file_loop;
	int	m_file_loopstart;
	int	m_file_looplength;
	int _ogg_open();

static size_t _mem_fread(void *buffer, size_t size, size_t count, void *fp);
static int _mem_fseek(void *fp, ogg_int64_t offset, int origin);
static int _mem_fclose(void *fp);
static long _mem_ftell(void *fp);

typedef struct {
	char	*buf;		// oggファイルバッファ
	int		size;		// サイズ
	int		cur;		// 位置
} MEM_STRUCT;
	MEM_STRUCT	m_oggmem;


static size_t _file_fread(void *buffer, size_t size, size_t count, void *fp);
static int _file_fseek(void *fp, ogg_int64_t offset, int origin);
static int _file_fclose(void *fp);
static long _file_ftell(void *fp);

typedef struct {
	LibSoundUtil::FILEDATA	*fp;
} FILE_STRUCT;
	FILE_STRUCT	m_oggfile;

};


#endif
