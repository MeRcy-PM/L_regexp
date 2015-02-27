#ifndef __BASE_H__
#define __BASE_H__
#include <iostream>
#include <vector>
#include <bitset>
#include <assert.h>
using std::vector;
using std::bitset;
using std::cout;
using std::endl;

void print_vector (vector<bool> *v) {
	unsigned int i = 0;
	for (i = 0; i < v->size (); i++) {
		if (v->at(i))
			cout << i << " ";
	}
}

#endif
