#ifndef	__LIBSOUNDSTORAGE_WAV_H__
#define	__LIBSOUNDSTORAGE_WAV_H__



class LibSoundStorage;

class LibSoundStorage_Wav {

public:
	LibSoundStorage_Wav( LibSoundStorage *storage );
	~LibSoundStorage_Wav();

	static bool checkdatatype( char *p );

	int open( int *pcmrate, int *pcmbits, int *pcmchannels );
	void close();
	void *read( int needbytes, int *readbytes );
	void seek( unsigned int pos );

	bool getfileloopsetting( int *st, int *len );

private:
	LibSoundStorage *m_storage;

// ここからファイルフォーマット別
#define	WAVE_NEED_DATASIZE	((4+4)+(4+4+4+2+2+4+4+2+2+2+128)+(4+4))
/*
4 byte	R' 'I' 'F' 'F'	RIFFヘッダ	　
4 byte	これ以降のファイルサイズ (ファイルサイズ - 8)	　	　
4 byte	W' 'A' 'V' 'E'	WAVEヘッダ	RIFFの種類がWAVEであることをあらわす
4 byte	f' 'm' 't' ' ' (←スペースも含む)	fmt チャンク	フォーマットの定義
4 byte	バイト数	fmt チャンクのバイト数	リニアPCM ならば 16(10 00 00 00)
2 byte	フォーマットID	参照	リニアPCM ならば 1(01 00)
2 byte	チャンネル数	　	モノラル ならば 1(01 00)ステレオ ならば 2(02 00)
4 byte	サンプリングレート	Hz	44.1kHz ならば 44100(44 AC 00 00)
4 byte	データ速度 (Byte/sec)	　	44.1kHz 16bit ステレオ ならば44100×2×2 = 176400(10 B1 02 00)
2 byte	ブロックサイズ (Byte/sample×チャンネル数)	　	16bit ステレオ ならば2×2 = 4(04 00)
2 byte	サンプルあたりのビット数 (bit/sample)	WAV フォーマットでは 8bit か 16bit	16bit ならば 16(10 00)
2 byte	拡張部分のサイズ	　	リニアPCMならば存在しない
n byte	拡張部分	　	リニアPCMならば存在しない
4 byte	d' 'a' 't' 'a'	data チャンク 参照	　
4 byte	バイト数n	波形データのバイト数	　
n byte	波形データ	参照
*/

	int	filesize;

	typedef struct {
		char	cWAVE[4];
		char	cfmt_[4];
		unsigned int	bytes;
		unsigned short	format_id;
		unsigned short	channel;
		unsigned int	samplerate;
		unsigned int	dataspeed;
		unsigned short	blocksize;
		unsigned short	samplebits;
		unsigned short	extsize;
	} WAVE_FMT;
	WAVE_FMT	fmt;

	typedef struct {
		char	cdata[4];
		unsigned int	pcmdata_len;
		unsigned int	pcmdata_pos_from_header;
	} WAVE_DATA;
	WAVE_DATA	dat;

	int _open( void *pRiffHead );

	typedef struct {
		LibSoundUtil::FILEDATA	*fp;
	} FILE_STRUCT;
	FILE_STRUCT	m_file;

	int _read( void *wbuf, unsigned int size );
	int read_cur;

};


#endif
