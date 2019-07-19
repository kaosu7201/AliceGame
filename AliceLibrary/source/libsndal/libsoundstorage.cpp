/*
** storage
** {file, memory}, {ogg,wave,pcm} を隠蔽する
*/


#include	"libsound.h"

LibSoundStorage::LibSoundStorage( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize ) {
	m_storagetype = storagetype;
	m_datatype = LibSoundUtil::DATATYPE_ERR;
	m_fOpen = false;
	m_bProc = false;
	pStorageClass = NULL;
	switch( m_storagetype ) {
		case LibSoundUtil::STORAGETYPE_FILE:
			strcpy( m_szFile, (char*)pData );
			m_datatype = _checkfiletype( m_szFile );
			break;
		case LibSoundUtil::STORAGETYPE_MEMORY:
			m_datatype = _checkdatatype( (char*)pData );
			m_memdata = pData;
			m_memdatasize = nDataSize;
			break;
	}
}

LibSoundStorage::~LibSoundStorage() {
	close();
}





LibSoundUtil::DATATYPE LibSoundStorage::_checkdatatype( char *p ) {

	if ( LibSoundStorage_Ogg::checkdatatype(p) ) { return LibSoundUtil::DATATYPE_OGG; }
	if ( LibSoundStorage_Wav::checkdatatype(p) ) { return LibSoundUtil::DATATYPE_WAV; }

	return LibSoundUtil::DATATYPE_PCM;
}

LibSoundUtil::DATATYPE LibSoundStorage::_checkfiletype( char *szFile ) {
	LibSoundUtil::FILEDATA *fp;
	fp = LibSoundUtil::fileopen( szFile );
	if ( !fp ) { return LibSoundUtil::DATATYPE_ERR; }
	LibSoundUtil::fileseek( fp, 0, SEEK_END ); int size = LibSoundUtil::filetell( fp ); LibSoundUtil::fileseek( fp, 0, SEEK_SET );

	// ファイルサイズは小さすぎないか？
	if ( size < 128 ) {
		LibSoundUtil::fileclose( fp );
		return LibSoundUtil::DATATYPE_ERR;
	}

	// 種別判断のために128byteだけ読み込む
	char head[128];
	LibSoundUtil::fileread( head, 1, sizeof( head ), fp );
	LibSoundUtil::fileclose( fp );

	return _checkdatatype( head );
}




int LibSoundStorage::open( int *pcmrate, int *pcmbits, int *pcmchannels ) {

	if ( !m_fOpen ) {
		switch( m_datatype ) {
			case LibSoundUtil::DATATYPE_OGG:
			{
				LibSoundStorage_Ogg *s = new LibSoundStorage_Ogg( this );
				s->open( &m_pcmdata_rate, &m_pcmdata_bits, &m_pcmdata_channels );
				pStorageClass = (void*)s;
				break;
			}
			case LibSoundUtil::DATATYPE_WAV:
			{
				LibSoundStorage_Wav *s = new LibSoundStorage_Wav( this );
				s->open( &m_pcmdata_rate, &m_pcmdata_bits, &m_pcmdata_channels );
				pStorageClass = (void*)s;
				break;
			}
			case LibSoundUtil::DATATYPE_PCM:	// 現状、bits/rateなどの情報が無いので作れない
				break;
		}
		m_fOpen = true;
	}

	if ( m_fOpen ) {
		if ( pcmrate )		{ *pcmrate = m_pcmdata_rate; }
		if ( pcmbits )		{ *pcmbits = m_pcmdata_bits; }
		if ( pcmchannels )	{ *pcmchannels = m_pcmdata_channels; }
		return 1;
	}
	return 0;
}

void LibSoundStorage::close() {
	if ( !m_fOpen ) { return; }
	m_fOpen = false;

	while( m_bProc ) {
		LibSoundUtil::SleepThread(1);
	}

	switch( m_datatype ) {
		case LibSoundUtil::DATATYPE_OGG:	{ reinterpret_cast<LibSoundStorage_Ogg*>(pStorageClass)->close(); break; }
		case LibSoundUtil::DATATYPE_WAV:	{ reinterpret_cast<LibSoundStorage_Wav*>(pStorageClass)->close(); break; }
		case LibSoundUtil::DATATYPE_PCM:	// 現状、bits/rateなどの情報が無いので作れない
			break;
	}
	if ( pStorageClass ) { delete pStorageClass; }

}


// pcmdataを得たい
// 得たpcmdataは必ずfreeすること
// needbytesが-1の場合は全読み込み
void *LibSoundStorage::read( int needbytes, int *readbytes ) {
	if ( !m_fOpen ) { return NULL; }
	void *p = NULL;

	m_bProc = true;
	switch( m_datatype ) {
		case LibSoundUtil::DATATYPE_OGG:	{ p = reinterpret_cast<LibSoundStorage_Ogg*>(pStorageClass)->read( needbytes, readbytes ); break; }
		case LibSoundUtil::DATATYPE_WAV:	{ p = reinterpret_cast<LibSoundStorage_Wav*>(pStorageClass)->read( needbytes, readbytes ); break; }
		case LibSoundUtil::DATATYPE_PCM:	// 現状、bits/rateなどの情報が無いので作れない
			break;
	}
	m_bProc = false;
	return p;
}

void LibSoundStorage::seek( unsigned int pos ) {
	if ( !m_fOpen ) { return; }

	m_bProc = true;
	switch( m_datatype ) {
		case LibSoundUtil::DATATYPE_OGG:	{ reinterpret_cast<LibSoundStorage_Ogg*>(pStorageClass)->seek( pos ); break; }
		case LibSoundUtil::DATATYPE_WAV:	{ reinterpret_cast<LibSoundStorage_Wav*>(pStorageClass)->seek( pos ); break; }
		case LibSoundUtil::DATATYPE_PCM:	// 現状、bits/rateなどの情報が無いので作れない
			break;
	}
	m_bProc = false;
}


bool LibSoundStorage::getfileloopsetting( int *st, int *len ) {
//	if ( !m_fOpen ) { return; }
	bool b = false;

	m_bProc = true;
	switch( m_datatype ) {
		case LibSoundUtil::DATATYPE_OGG:	{ b = reinterpret_cast<LibSoundStorage_Ogg*>(pStorageClass)->getfileloopsetting( st, len ); break; }
		case LibSoundUtil::DATATYPE_WAV:	{ b = reinterpret_cast<LibSoundStorage_Wav*>(pStorageClass)->getfileloopsetting( st, len ); break; }
		case LibSoundUtil::DATATYPE_PCM:	// 現状、bits/rateなどの情報が無いので作れない
			break;
	}
	m_bProc = false;
	return b;
}

