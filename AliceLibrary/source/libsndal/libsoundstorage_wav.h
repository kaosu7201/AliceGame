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

// ��������t�@�C���t�H�[�}�b�g��
#define	WAVE_NEED_DATASIZE	((4+4)+(4+4+4+2+2+4+4+2+2+2+128)+(4+4))
/*
4 byte	R' 'I' 'F' 'F'	RIFF�w�b�_	�@
4 byte	����ȍ~�̃t�@�C���T�C�Y (�t�@�C���T�C�Y - 8)	�@	�@
4 byte	W' 'A' 'V' 'E'	WAVE�w�b�_	RIFF�̎�ނ�WAVE�ł��邱�Ƃ�����킷
4 byte	f' 'm' 't' ' ' (���X�y�[�X���܂�)	fmt �`�����N	�t�H�[�}�b�g�̒�`
4 byte	�o�C�g��	fmt �`�����N�̃o�C�g��	���j�APCM �Ȃ�� 16(10 00 00 00)
2 byte	�t�H�[�}�b�gID	�Q��	���j�APCM �Ȃ�� 1(01 00)
2 byte	�`�����l����	�@	���m���� �Ȃ�� 1(01 00)�X�e���I �Ȃ�� 2(02 00)
4 byte	�T���v�����O���[�g	Hz	44.1kHz �Ȃ�� 44100(44 AC 00 00)
4 byte	�f�[�^���x (Byte/sec)	�@	44.1kHz 16bit �X�e���I �Ȃ��44100�~2�~2 = 176400(10 B1 02 00)
2 byte	�u���b�N�T�C�Y (Byte/sample�~�`�����l����)	�@	16bit �X�e���I �Ȃ��2�~2 = 4(04 00)
2 byte	�T���v��������̃r�b�g�� (bit/sample)	WAV �t�H�[�}�b�g�ł� 8bit �� 16bit	16bit �Ȃ�� 16(10 00)
2 byte	�g�������̃T�C�Y	�@	���j�APCM�Ȃ�Α��݂��Ȃ�
n byte	�g������	�@	���j�APCM�Ȃ�Α��݂��Ȃ�
4 byte	d' 'a' 't' 'a'	data �`�����N �Q��	�@
4 byte	�o�C�g��n	�g�`�f�[�^�̃o�C�g��	�@
n byte	�g�`�f�[�^	�Q��
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
