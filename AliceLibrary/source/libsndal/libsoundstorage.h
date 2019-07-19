#ifndef	__LIBSOUNDSTORAGE_H__
#define	__LIBSOUNDSTORAGE_H__


#include "libsoundstorage_ogg.h"
#include "libsoundstorage_wav.h"


class LibSoundStorage {

public:
	LibSoundStorage( LibSoundUtil::STORAGETYPE storagetype, void *pData, int nDataSize );
	~LibSoundStorage();

	LibSoundUtil::STORAGETYPE m_storagetype;
	LibSoundUtil::DATATYPE	m_datatype;

	char	m_szFile[512];
	void	*m_memdata;
	int		m_memdatasize;

	int open( int *pcmrate, int *pcmbits, int *pcmchannels );
	void close();
	void *read( int needbytes, int *readbytes );
	void seek( unsigned int pos );

	bool getfileloopsetting( int *st, int *len );

	bool	m_fOpen;
	int m_pcmdata_rate, m_pcmdata_bits, m_pcmdata_channels;

	void *pStorageClass;

private:
	LibSoundUtil::DATATYPE _checkdatatype( char *p );
	LibSoundUtil::DATATYPE _checkfiletype( char *szFile );


	bool	m_bProc;

};


#endif
