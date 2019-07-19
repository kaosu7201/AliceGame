/*
** storage, ogg
*/

#include "libsound.h"

// file access.

size_t LibSoundStorage_Ogg::_file_fread(void *buffer, size_t size, size_t maxcount, void *fp) {
	FILE_STRUCT *p = (FILE_STRUCT*)fp;
	if ( buffer == NULL ) { return 0; }
	return LibSoundUtil::fileread( buffer, size, maxcount, p->fp );
}
int LibSoundStorage_Ogg::_file_fseek(void *fp, ogg_int64_t offset, int origin) {
	FILE_STRUCT *p = (FILE_STRUCT*)fp;
	return LibSoundUtil::fileseek( p->fp, (long)offset, origin );
}
int LibSoundStorage_Ogg::_file_fclose(void *fp) {
	FILE_STRUCT *p = (FILE_STRUCT*)fp;
	return LibSoundUtil::fileclose( p->fp );
}
long LibSoundStorage_Ogg::_file_ftell(void *fp) {
	FILE_STRUCT *p = (FILE_STRUCT*)fp;
	return LibSoundUtil::filetell( p->fp );
}




// memory access.

size_t LibSoundStorage_Ogg::_mem_fread(void *buffer, size_t size, size_t maxcount, void *fp) {
	MEM_STRUCT *p = (MEM_STRUCT*)fp;
	if ( buffer == NULL ) { return 0; }

	int resSize = p->size - p->cur;
	size_t count = resSize / size;
	if ( count > maxcount ) { count = maxcount; }

	memcpy( buffer, (char*)&p->buf[p->cur], size * count );

	p->cur += size * count;

	return count;
}
int LibSoundStorage_Ogg::_mem_fseek(void *fp, ogg_int64_t offset, int origin) {
	MEM_STRUCT *p = (MEM_STRUCT*)fp;

	switch( origin ) {
		case SEEK_CUR: { p->cur += (int)offset; break; }
		case SEEK_END: { p->cur = p->size + (int)offset; break; }
		case SEEK_SET: { p->cur = (int)offset; break; }
		default:
			return -1;
	}
	if ( p->cur > p->size ) {
		p->cur = p->size;
		return -1;
	}
	if ( p->cur < 0 ) {
		p->cur = 0;
		return -1;
	}
	return 0;
}
int LibSoundStorage_Ogg::_mem_fclose(void *fp) {
	MEM_STRUCT *p = (MEM_STRUCT*)fp;
	return 0;
}
long LibSoundStorage_Ogg::_mem_ftell(void *fp) {
	MEM_STRUCT *p = (MEM_STRUCT*)fp;
	return p->cur;
}

bool LibSoundStorage_Ogg::getfileloopsetting( int *st, int *len ) {
	if ( m_file_loop ) {
		*st		= m_file_loopstart;
		*len	= m_file_looplength;
		return true;
	}
	return false;
}







int LibSoundStorage_Ogg::_ogg_open() {
	int err = 0;
	switch( m_storage->m_storagetype ) {
		case LibSoundUtil::STORAGETYPE_FILE:
		{
			ov_callbacks callbacks = {
				(size_t (*)(void *, size_t, size_t, void *)) _file_fread,
				(int (*)(void *, ogg_int64_t, int))          _file_fseek,
				(int (*)(void *))                            _file_fclose,
				(long (*)(void *))                           _file_ftell
			};
			LibSoundUtil::FILEDATA *fp = LibSoundUtil::fileopen( m_storage->m_szFile );
			if ( !fp ) {
				err = -1;
			} else {
				m_oggfile.fp = fp;
				err = ov_open_callbacks((void *)&m_oggfile, &m_ovf, NULL, 0, callbacks);
				if ( err ) { LibSoundUtil::fileclose( fp ); }
			}
			break;
		}
		case LibSoundUtil::STORAGETYPE_MEMORY:
		// stream, staticで使われる
		{
			ov_callbacks callbacks = {
				(size_t (*)(void *, size_t, size_t, void *)) _mem_fread,
				(int (*)(void *, ogg_int64_t, int))          _mem_fseek,
				(int (*)(void *))                            _mem_fclose,
				(long (*)(void *))                           _mem_ftell
			};
			m_oggmem.buf = (char*)m_storage->m_memdata;
			m_oggmem.cur = 0;
			m_oggmem.size = m_storage->m_memdatasize;
			err = ov_open_callbacks((void *)&m_oggmem, &m_ovf, NULL, 0, callbacks);
			break;
		}
	}

	return err;
}







LibSoundStorage_Ogg::LibSoundStorage_Ogg( LibSoundStorage *storage ) {
	m_storage = storage;
}

LibSoundStorage_Ogg::~LibSoundStorage_Ogg() {
	close();
}

bool LibSoundStorage_Ogg::checkdatatype( char *p ) {
	if ( memcmp( p, "OggS", 4 ) == 0 ) { return true; }
	return false;
}


int LibSoundStorage_Ogg::open( int *pcmrate, int *pcmbits, int *pcmchannels ) {
	int error = _ogg_open();
	if ( error != 0 ) {
		switch( error ) {
			case OV_EREAD:       break;
			case OV_ENOTVORBIS:  break;
			case OV_EVERSION:    break;
			case OV_EBADHEADER:  break;
			case OV_EFAULT:      break;
			default:             break;
		}
		return 0; // エラー
	}

	// ファイル情報の取得
	vorbis_info *ovi = ov_info( &m_ovf, -1 );

	// format情報
	*pcmrate  = ovi->rate;
	*pcmbits = 16;
	*pcmchannels = ovi->channels;

	// ループポジションを得ておく
	m_file_loop = false;
	{
		int fSt = 0, fLen = 0;
		OggVorbis_File *povf = (OggVorbis_File *)( &m_ovf );

		// ファイル情報の取得
		vorbis_info *ovi = ov_info( povf, -1 );
		// コメントを検索してループ位置を決める
		for( int i = 0; i < povf->vc->comments; i++ ) {
			char *cc = povf->vc->user_comments[i];
			if ( strncmp( cc, "LOOPSTART=", 10 ) == 0 ) {
				m_file_loopstart = atoi( &cc[10] ); fSt = 1;
			} else if ( strncmp( cc, "LOOPLENGTH=", 11 ) == 0 ) {
				m_file_looplength = atoi( &cc[11] ); fLen = 1;
			}
		}
		if ( fSt && fLen ) {
			m_file_loop = true;
		}
	}

	return 1;
}

void LibSoundStorage_Ogg::close() {
	ov_clear( &m_ovf );
}

void *LibSoundStorage_Ogg::read( int needbytes, int *readbytes ) {

	int tmpbase = 4096, tmplen = tmpbase * 16, tmppos = 0;
	char *tmp = (char*)malloc( tmplen );
	for(;;){
		if ( tmppos >= needbytes && needbytes >= 0 ) { break; }
		int bitstream = 0;
		if ( tmppos + tmpbase > tmplen ) {
			tmplen += tmpbase * 16;
			tmp = (char*)realloc( tmp, tmplen );
		}
		int readSize = ov_read(
			&m_ovf,
			&tmp[tmppos],
			tmpbase,
			0,		// リトルエンディアン
			2,		// WORDは16bit
			1,		// 符号有り
			&bitstream
		);
		if ( !readSize ) { break; }
		tmppos += readSize;
	}
	// tmp に tmppos の分だけデータが入っている

	*readbytes = tmppos;
	return tmp;
}

void LibSoundStorage_Ogg::seek( unsigned int pos ) {
	ov_pcm_seek( &m_ovf, pos );
}



