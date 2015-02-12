#include "stree.h"
#include "dfa.h"
#ifdef __DEBUG__
#define __PRINT__
#endif

void testsuite (void);
int main (int argc, char **argv)
{
#ifndef TEST
	if (argc <= 2) {
		printf ("Usage: %s expr text\n", argv[0]);
		exit (1);
	}
	grep (argv[1], argv[2]);
	printf ("Match Result is : %s\n", match_string);
	stree_finit ();
	if (match_string) {
		free (match_string);
		match_string = NULL;
	}
#else
	if (argc != 1)
		printf ("Test mode, will ignore all argument.\n");

	testsuite ();
#endif

	return 0;
}

int grep (char *arg1, char *arg2)
{
	build_syntax_tree (arg1);
	compute_property ();
	build_graph ();
	compute_nfa_trans ();
#ifdef __PRINT__
	print_stree (NULL);
	print_graph ();
#endif
	match_expr (arg2);
	return 0;
}

void test_case (char *rexp, char *string, char *expect)
{
	grep (rexp, string);
	if (strcmp (match_string, expect) != 0)
		printf ("COMPARE FAIL. Rexp is %s\nString is %s\nE is %sResult is %s\n", rexp, string, expect, match_string);
	else
		printf ("COMPARE COMPLETE.\n");
	stree_finit ();
	if (match_string) {
		free (match_string);
		match_string = NULL;
	}

	return;
}

void testsuite ()
{
	test_case ("a*b", "aaab", "aaab");
	test_case ("(a|b)*", "aabbb", "aabbb");
	test_case ("(a)", "a", "a");
}
