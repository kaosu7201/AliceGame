#ifndef	__LIB_TEXTURE_PACK_H__
#define	__LIB_TEXTURE_PACK_H__

#include <stdio.h>
#include <string.h>
#include <cstdint>
#define	TEXTUREPACK_TEXTUREDATA		int


typedef struct {
	void	*prev;
	void	*next;
	void	*header;
	void	*data;
	TEXTUREPACK_TEXTUREDATA		*texlist;
	// 動作設定
	float	_scaleRate;		// 高解像度版とする場合の描画縮尺レート
} LIBTEXPACK_DATA;

void LibTexPackInit();
void LibTexPackRelease();
LIBTEXPACK_DATA *LibTexPackSetBin( void *mem, int size );
void LibTextPackReleaseBin( LIBTEXPACK_DATA *bin );
int LibTexPackGetTextureNum( LIBTEXPACK_DATA *bin );
void LibTexPackSetTextureId( LIBTEXPACK_DATA *bin, int texture_num, TEXTUREPACK_TEXTUREDATA texture_id );
TEXTUREPACK_TEXTUREDATA LibTexPackGetTexture( LIBTEXPACK_DATA *bin, int texture_num );
int LibTexPackGetDataNum(LIBTEXPACK_DATA *bin);

int LibTexPackGetData( LIBTEXPACK_DATA *bin, const char *pszId, TEXTUREPACK_TEXTUREDATA *pnTex, int *pnSX, int *pnSY, int *pnSW, int *pnSH, int *pnOX, int *pnOY, int *pnOW, int *pnOH );
int LibTexPackGetData(LIBTEXPACK_DATA *bin, const char *pszId, int *pnOW, int *pnOH);
int LibTexPackGetData(LIBTEXPACK_DATA *bin, int index, TEXTUREPACK_TEXTUREDATA *pnTex, int *pnSX, int *pnSY, int *pnSW, int *pnSH, int *pnOX, int *pnOY, int *pnOW, int *pnOH);
int LibTexPackGetData(LIBTEXPACK_DATA *bin, int index, int *pnOW, int *pnOH);

// 合った場合はIndex番号、無かった場合は-1が返る
int LibTexPackGetIndex( LIBTEXPACK_DATA *bin, const char *pszId );

// Index番号からId名を得る。無い場合はNULLが返る
char *LibTexPackGetIDName(LIBTEXPACK_DATA *bin, int index);


#endif
