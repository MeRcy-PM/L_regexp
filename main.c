#include "stree.h"
#include "dfa.h"

void test_print_stree ()
{
	syntax_tree = build_stree_node ('|');
	syntax_tree->lchild = build_stree_node ('|');
	syntax_tree->rchild = build_stree_node ('c');
	syntax_tree->lchild->lchild = build_stree_node ('a');
	syntax_tree->lchild->rchild = build_stree_node ('b');
	print_stree (NULL);
}

void test_set_union ()
{
	int i;
	int a[3] = {1, 2, 3};
	int b[4] = {2, 3, 2, 5};
	set_p aset, bset, ahead, bhead, uset;
	ahead = aset = new_set_node ();
	aset->value = a[0];
	for (i = 1; i < 3; i++) {
		aset->next = new_set_node ();
		aset = aset->next;
		aset->value = a[i];
	}

	bhead = bset = new_set_node ();
	bset->value = b[0];
	for (i = 1; i < 4; i++) {
		bset->next = new_set_node ();
		bset = bset->next;
		bset->value = b[i];
	}
	print_set ("a_set", ahead);
	printf ("\n");
	print_set ("b_set", bhead);
	printf ("\n");
	uset = set_union (ahead, bhead);
	print_set ("union_set", uset);
	printf ("\n");
}

int main (int argc, char **argv)
{
	//test_print_stree ();
	if (argc == 1) {
		printf ("Usage: %s expr\n", argv[0]);
		exit (1);
	}
	build_syntax_tree (argv[1]);
	compute_nullable ();
	compute_first_op ();
	compute_last_op ();
	build_graph ();
	print_stree (NULL);
	print_graph ();
	stree_finit ();
	//test_set_union ();
	return 0;
}

