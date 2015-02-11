#include "stree.h"
#include "dfa.h"
#ifndef TEST
#include "test.h"
#endif

int main (int argc, char **argv)
{
#ifndef TEST
	grep (argc, argv);
#else
	testsuite ();
#endif
}

int grep (int argc, char **argv)
{
	if (argc <= 2) {
		printf ("Usage: %s expr text\n", argv[0]);
		exit (1);
	}
	build_syntax_tree (argv[1]);
	compute_property ();
	build_graph ();
	compute_nfa_trans ();
	print_stree (NULL);
	print_graph ();
	match_expr (argv[2]);
	printf ("Match Result is : %s\n", match_string);
	stree_finit ();
	if (match_string)
		free (match_string);
	return 0;
}

