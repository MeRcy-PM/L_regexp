#include "bitmap.h"

bitmap_p new_bitmap (int size)
{
	bitmap_p new = (bitmap_p)malloc (sizeof (struct bitmap));
	new->size = size;
	new->map = (int *)calloc (size / 8 + 1, sizeof (char));
}

void bit_set (bitmap_p bitmap, int bit)
{
	if (bit > bitmap->size) {
		printf ("Set bit error.\n");
		exit (1);
	}

	int rbit = (bit - 1) % 32;
	int rfield = (bit - 1) / 32;
	bitmap->map[rfield] |= (1 << rbit);
}

void print_bitmap (bitmap_p bitmap)
{
	int i, field = bitmap->size / 32;
	int bit = bitmap->size % 32;
	printf ("Total status is %d.\n", bitmap->size);
	for (i = 0; i < field; i++) {
		printf ("%x\n", bitmap->map[i]);
	}
	printf ("%x\n", bitmap->map[field] & (0x7fffffff >> (32 - bit - 1)));
}

