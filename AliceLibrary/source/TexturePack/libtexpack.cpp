/*
** lib texture pack
**  pngmarge で出力したテクスチャアトラスデータを扱うライブラリ
*/

#include <stdio.h>
#include <stdlib.h>
#include "libtexpack.h"


#ifndef NULL
	#define	NULL	(0)
#endif // NULL



static LIBTEXPACK_DATA *root = NULL;


typedef struct {
	/*long*/int32_t	ver;				// バージョン
	/*long*/int32_t	texnum;				// テクスチャ枚数
	/*long*/int32_t	datanum;			// データの個数
	/*long*/int32_t	idlen;				// ID名の最大サイズ(ASCIIZ)
} INFHEADER;

typedef struct {
	/*long*/int32_t	nTex;				// テクスチャ番号(0～)
	/*long*/int32_t	sx, sy, sw, sh;		// 切り出しUV
	/*long*/int32_t	ox, oy, ow, oh;		// 本来のUV
} INFDATA_IN;


void LibTexPackInit() {
	root = NULL;
}

void LibTexPackRelease() {
	LIBTEXPACK_DATA *p = root;
	while( p ) {
		LIBTEXPACK_DATA *next = (LIBTEXPACK_DATA *)p->next;
		LibTextPackReleaseBin( p );
		p = next;
	}
	root = NULL;
}

// memはsetしたら破棄して良い
LIBTEXPACK_DATA *LibTexPackSetBin( void *mem, int size ) {
	// 最後を得る
	LIBTEXPACK_DATA *p = root;
	LIBTEXPACK_DATA *prev = root;
	while( p && p->next ) {
//		prev = p;
		p = (LIBTEXPACK_DATA *)p->next;
	}
	if ( !root ) {
		root = (LIBTEXPACK_DATA*)malloc( sizeof( LIBTEXPACK_DATA ) );
		p = (LIBTEXPACK_DATA*)root;
		prev = NULL;
	} else {
		p->next = (LIBTEXPACK_DATA*)malloc( sizeof( LIBTEXPACK_DATA ) );
		prev = p;
		p = (LIBTEXPACK_DATA*)p->next;
	}

	INFHEADER *header = (INFHEADER*)mem;
	char *memdata = NULL;
	// バージョン確認
	if ( header->ver == 1 ) {
		memdata = (char*)malloc( size );
		memcpy( memdata, mem, size );
	}

	p->next			= NULL;
	p->prev			= prev;
	p->header		= memdata;
	p->data			= &memdata[sizeof( INFHEADER )];
	p->texlist		= (TEXTUREPACK_TEXTUREDATA*)malloc( sizeof( TEXTUREPACK_TEXTUREDATA ) * header->texnum );
	p->_scaleRate	= 1.0f;

	return p;
}

void LibTextPackReleaseBin( LIBTEXPACK_DATA *bin ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	TEXTUREPACK_TEXTUREDATA *texlist = (TEXTUREPACK_TEXTUREDATA*)bin->texlist;
	LIBTEXPACK_DATA *prev = (LIBTEXPACK_DATA*)bin->prev;
	LIBTEXPACK_DATA *next = (LIBTEXPACK_DATA*)bin->next;

	free( header );
	free( texlist );
	free( bin );

	if ( prev ) { prev->next = next; } else { root = NULL; }
	if ( next ) { next->prev = prev; }
}

int LibTexPackGetTextureNum( LIBTEXPACK_DATA *bin ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	return header->texnum;
}

void LibTexPackSetTextureId( LIBTEXPACK_DATA *bin, int texture_num, TEXTUREPACK_TEXTUREDATA texture_id ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	TEXTUREPACK_TEXTUREDATA *texlist		= (TEXTUREPACK_TEXTUREDATA*)bin->texlist;

	if ( 0 <= texture_num && texture_num < header->texnum ) {
		texlist[texture_num] = texture_id;
	}
}

TEXTUREPACK_TEXTUREDATA LibTexPackGetTexture( LIBTEXPACK_DATA *bin, int texture_num ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	TEXTUREPACK_TEXTUREDATA *texlist		= (TEXTUREPACK_TEXTUREDATA*)bin->texlist;
	if ( 0 <= texture_num && texture_num < header->texnum ) {
		return texlist[texture_num];
	}
	return 0;
}

int LibTexPackGetDataNum(LIBTEXPACK_DATA *bin) {
	INFHEADER *header = (INFHEADER*)bin->header;
	return header->datanum;
}


static int _SearchDataIndex( LIBTEXPACK_DATA *bin, const char *pszId ) {
	INFHEADER *header	= (INFHEADER*)bin->header;

	char *datahead	= (char*)bin->data;
	int datasize = sizeof( INFDATA_IN ) + header->idlen;
#if 0
	int i;
	// ID名で並べてあるからクイックサーチ可能だけど、とりあえず全サーチで実装
	for ( i = 0; i < header->datanum; i++ ) {
		char *pid = &datahead[ datasize * i + sizeof( INFDATA_IN ) ];
		if ( strcmp( pid, pszId ) == 0 ) {
			return i;
		}
	}
#else
	// クイックサーチ（ソートされていることが条件）
	int st = 0, ed = header->datanum, t, r;
	for(;;) {
		t = st + ( ed - st ) / 2;
		char *pid = &datahead[ datasize * t + sizeof( INFDATA_IN ) ];
		r = strcmp( pid, pszId );
		if ( r == 0 ) { return t; }	// データがあった
		if ( st == ed ) { break; }	// データが無かった
		if ( r > 0 ) { if ( t == ed ) { break; } ed = t; } else { if ( t == st ) { break; } st = t; }
	}

#endif

	// data無し
	return -1;
}

static INFDATA_IN *_SearchData( LIBTEXPACK_DATA *bin, int index ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	char *datahead	= (char*)bin->data;
	int datasize = sizeof( INFDATA_IN ) + header->idlen;
	if ( index < 0 ) { return NULL; }
	return (INFDATA_IN *)&datahead[ datasize * index ];
}

static INFDATA_IN *_SearchData( LIBTEXPACK_DATA *bin, const char *pszId ) {
	return _SearchData( bin, _SearchDataIndex( bin, pszId ) );
}


int LibTexPackGetData( LIBTEXPACK_DATA *bin, const char *pszId, TEXTUREPACK_TEXTUREDATA *pnTex, int *pnSX, int *pnSY, int *pnSW, int *pnSH, int *pnOX, int *pnOY, int *pnOW, int *pnOH ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	INFDATA_IN *data = _SearchData( bin, pszId );
	if ( data ) {
		TEXTUREPACK_TEXTUREDATA *texlist = (TEXTUREPACK_TEXTUREDATA*)bin->texlist;
		if ( pnTex )	{ *pnTex = texlist[ data->nTex ]; }
		if ( pnSX )		{ *pnSX = data->sx; }
		if ( pnSY )		{ *pnSY = data->sy; }
		if ( pnSW )		{ *pnSW = data->sw; }
		if ( pnSH )		{ *pnSH = data->sh; }
		if ( pnOX )		{ *pnOX = data->ox; }
		if ( pnOY )		{ *pnOY = data->oy; }
		if ( pnOW )		{ *pnOW = data->ow; }
		if ( pnOH )		{ *pnOH = data->oh; }
		return 1;
	}
	// 無かった
	return 0;
}

int LibTexPackGetData(LIBTEXPACK_DATA *bin, const char *pszId, int *pnOW, int *pnOH) {
	return LibTexPackGetData(bin, pszId, NULL, NULL, NULL, NULL, NULL, NULL, NULL, pnOW, pnOH);
}

int LibTexPackGetData( LIBTEXPACK_DATA *bin, int index, TEXTUREPACK_TEXTUREDATA *pnTex, int *pnSX, int *pnSY, int *pnSW, int *pnSH, int *pnOX, int *pnOY, int *pnOW, int *pnOH ) {
	INFHEADER *header	= (INFHEADER*)bin->header;
	INFDATA_IN *data = _SearchData( bin, index );
	if ( data ) {
		TEXTUREPACK_TEXTUREDATA *texlist = (TEXTUREPACK_TEXTUREDATA*)bin->texlist;
		if ( pnTex )	{ *pnTex = texlist[ data->nTex ]; }
		if ( pnSX )		{ *pnSX = data->sx; }
		if ( pnSY )		{ *pnSY = data->sy; }
		if ( pnSW )		{ *pnSW = data->sw; }
		if ( pnSH )		{ *pnSH = data->sh; }
		if ( pnOX )		{ *pnOX = data->ox; }
		if ( pnOY )		{ *pnOY = data->oy; }
		if ( pnOW )		{ *pnOW = data->ow; }
		if ( pnOH )		{ *pnOH = data->oh; }
		return 1;
	}
	// 無かった
	return 0;
}

int LibTexPackGetData(LIBTEXPACK_DATA *bin, int index, int *pnOW, int *pnOH) {
	return LibTexPackGetData(bin, index, NULL, NULL, NULL, NULL, NULL, NULL, NULL, pnOW, pnOH);
}


// 合った場合はIndex番号、無かった場合は-1が返る
int LibTexPackGetIndex( LIBTEXPACK_DATA *bin, const char *pszId ) {
	return _SearchDataIndex( bin, pszId );
}

// Index番号からId名を得る。無い場合はNULLが返る
char *LibTexPackGetIDName(LIBTEXPACK_DATA *bin, int index) {
	INFHEADER *header = (INFHEADER*)bin->header;
	char *datahead = (char*)bin->data;
	int datasize = sizeof(INFDATA_IN) + header->idlen;
	if (index < 0 || index >= header->datanum) { return NULL; }
	char *pid = &datahead[datasize * index + sizeof(INFDATA_IN)];
	return pid;
}

