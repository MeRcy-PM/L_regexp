#ifndef __BASE_H__
#define __BASE_H__
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <stdlib.h>
using std::cout;
using std::endl;
using std::vector;
using std::string;

#define ERROR(s) \
{ \
	printf (s);\
	exit (1);\
}

#endif
