#ifndef __BITMAP_H__
#define __BITMAP_H__
#include "base.h"

struct bitmap {
	int *map;
	int size;
};
typedef struct bitmap * bitmap_p;

extern void bit_set (bitmap_p,int);
extern bitmap_p new_bitmap (int);
#endif
