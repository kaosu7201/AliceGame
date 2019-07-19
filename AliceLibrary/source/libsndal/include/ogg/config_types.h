#ifndef __CONFIG_TYPES_H__
#define __CONFIG_TYPES_H__

// Android NDK's config_types.h


#define _ogg_malloc  malloc
#define _ogg_calloc  calloc
#define _ogg_realloc realloc
#define _ogg_free    free

#include <stdint.h>

typedef short ogg_int16_t;
typedef unsigned short ogg_uint16_t;
//typedef long ogg_int32_t;
//typedef unsigned long ogg_uint32_t;
typedef int ogg_int32_t;
typedef unsigned int ogg_uint32_t;
typedef long long ogg_int64_t;


// tremor—p
//#include <malloc.h>
//#define rint(x)   (floor((x)+0.5f)) 
#define NO_FLOAT_MATH_LIB
//#define FAST_HYPOT(a, b) sqrt((a)*(a) + (b)*(b))
#define LITTLE_ENDIAN 1
#define BYTE_ORDER LITTLE_ENDIAN


#endif
