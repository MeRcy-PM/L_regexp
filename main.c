#include "stree.h"

void test_print_stree ()
{
	syntax_tree = build_stree_node ('|');
	syntax_tree->lchild = build_stree_node ('|');
	syntax_tree->rchild = build_stree_node ('c');
	syntax_tree->lchild->lchild = build_stree_node ('a');
	syntax_tree->lchild->rchild = build_stree_node ('b');
	print_stree (NULL);
}

int main (int argc, char **argv)
{
	//test_print_stree ();
	if (argc == 1) {
		printf ("Usage: %s expr\n", argv[0]);
		exit (1);
	}
	build_syntax_tree (argv[1]);
	print_stree (NULL);
	return 0;
}

