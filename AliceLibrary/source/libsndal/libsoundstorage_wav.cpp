/*
** storage, wav
*/

#include "libsound.h"


LibSoundStorage_Wav::LibSoundStorage_Wav( LibSoundStorage *storage ) {
	m_storage = storage;
}

LibSoundStorage_Wav::~LibSoundStorage_Wav() {
	close();
}

bool LibSoundStorage_Wav::checkdatatype( char *p ) {
	if ( memcmp( p, "RIFF", 4 ) == 0 ) { return true; }
	return false;
}



int LibSoundStorage_Wav::_open( void *pRiffHead ) {
	char *p = (char*)pRiffHead;

	int fsize = 0, n = 0, s;
	// RIFFチェック
	if ( memcmp( p, "RIFF", 4 ) != 0 ) {return 0; }
	n += 4;

	// ファイルサイズ取得
	s = 4; memcpy( &fsize, &p[n], s ); n += s;

	// WAVEチェック
	s = 4; memcpy( &fmt.cWAVE, &p[n], s ); n += s;
	if ( memcmp( fmt.cWAVE, "WAVE", 4 ) != 0 ) { return 0; }

	// fmt チェック
	s = 4; memcpy( &fmt.cfmt_, &p[n], s ); n += s;
	if ( memcmp( fmt.cfmt_, "fmt ", 4 ) != 0 ) { return 0; }

	// wave-struct
	s = 4; memcpy( &fmt.bytes, &p[n], s ); n += s;
	s = 2; memcpy( &fmt.format_id, &p[n], s ); n += s;
	s = 2; memcpy( &fmt.channel, &p[n], s ); n += s;
	s = 4; memcpy( &fmt.samplerate, &p[n], s ); n += s;
	s = 4; memcpy( &fmt.dataspeed, &p[n], s ); n += s;
	s = 2; memcpy( &fmt.blocksize, &p[n], s ); n += s;
	s = 2; memcpy( &fmt.samplebits, &p[n], s ); n += s;
	if ( fmt.format_id == 1 ) {
		// riner pcm
	} else {
		// extend
		s = 2; memcpy( &fmt.extsize, &p[n], s ); n += s;
		s = fmt.extsize; n += s;
	}

	// dataチェック
	s = 4; memcpy( &dat.cdata, &p[n], s ); n += s;
	if ( memcmp( dat.cdata, "data", 4 ) != 0 ) { return 0; }

	// data-struct
	s = 4; memcpy( &dat.pcmdata_len, &p[n], s ); n += s;
	dat.pcmdata_pos_from_header = n;

	return fsize;
}


int LibSoundStorage_Wav::open( int *pcmrate, int *pcmbits, int *pcmchannels ) {

	switch( m_storage->m_storagetype ) {
		case LibSoundUtil::STORAGETYPE_FILE:
		{
			LibSoundUtil::FILEDATA *fp = LibSoundUtil::fileopen( m_storage->m_szFile );
			if ( !fp ) { return 0; }
			m_file.fp = fp;
			// headerとして必要な分だけ読み込み
			char tmp[WAVE_NEED_DATASIZE];
			LibSoundUtil::fileread( tmp, 1, WAVE_NEED_DATASIZE, m_file.fp );
			if ( !_open( tmp ) ) { return 0; }
			// ファイル位置をpcmdata先頭にシーク
			LibSoundUtil::fileseek( m_file.fp, dat.pcmdata_pos_from_header + 0, SEEK_SET );
			break;
		}
		case LibSoundUtil::STORAGETYPE_MEMORY:
		{
			if ( !_open( m_storage->m_memdata ) ) { return 0; }
			break;
		}
	}
	read_cur = 0;

	// format情報
	*pcmrate		= fmt.samplerate;
	*pcmbits		= fmt.samplebits;
	*pcmchannels	= fmt.channel;

	return 1;
}

void LibSoundStorage_Wav::close() {
	switch( m_storage->m_storagetype ) {
		case LibSoundUtil::STORAGETYPE_FILE:
		{
			LibSoundUtil::fileclose( m_file.fp );
			break;
		}
	}
}

int LibSoundStorage_Wav::_read( void *wbuf, unsigned int size ) {
	if ( read_cur + size > dat.pcmdata_len ) {
		size = dat.pcmdata_len - read_cur;
	}
	if ( size < 0 ) { size = 0; }
	if ( size ) {
		switch( m_storage->m_storagetype ) {
			case LibSoundUtil::STORAGETYPE_FILE:
			{
				LibSoundUtil::fileread( wbuf, 1, size, m_file.fp );
				break;
			}
			case LibSoundUtil::STORAGETYPE_MEMORY:
			{
				char *p = (char*)(m_storage->m_memdata);
				memcpy( wbuf, &p[dat.pcmdata_pos_from_header + read_cur], size );
				break;
			}
		}
		read_cur += size;
	}

	return size;
}


// needbytesが-1の場合は全読み込み
void *LibSoundStorage_Wav::read( int needbytes, int *readbytes ) {

	int tmpbase = 4096*1024, tmplen = tmpbase * 16, tmppos = 0;
	char *tmp = (char*)malloc( tmplen );
	for(;;){
		if ( tmppos >= needbytes && needbytes >= 0 ) { break; }
		int bitstream = 0;
		if ( tmppos + tmpbase > tmplen ) {
			tmplen += tmpbase * 16;
			tmp = (char*)realloc( tmp, tmplen );
		}
		int readSize = _read(
			&tmp[tmppos],
			tmpbase
		);
		if ( !readSize ) { break; }
		tmppos += readSize;
	}
	// tmp に tmppos の分だけデータが入っている
	tmp = (char*)realloc( tmp, tmppos );

	*readbytes = tmppos;
	return tmp;
}

void LibSoundStorage_Wav::seek( unsigned int pos ) {
	switch( m_storage->m_storagetype ) {
		case LibSoundUtil::STORAGETYPE_FILE:
		{
			LibSoundUtil::fileseek( m_file.fp, dat.pcmdata_pos_from_header + pos, SEEK_SET );
			break;
		}
		case LibSoundUtil::STORAGETYPE_MEMORY:
		{
			break;
		}
	}
	read_cur = pos;
}

bool LibSoundStorage_Wav::getfileloopsetting( int *st, int *len ) {
	return false;
}

