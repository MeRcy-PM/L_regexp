#include "stack.hpp"
#include "stree.h"

int main (int argc, char **argv)
{
	syntax_tree stree;
	stree.build_tree (argv[1]);
	stree.print_syntax_tree ();
	return 0;
}

