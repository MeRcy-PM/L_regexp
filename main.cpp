#include <stdio.h>
#include "stack.hpp"
#include "stree.hpp"
#include "dfa.hpp"
#include "matcher.hpp"
#include <stdarg.h>

void inline print_help ()
{
	printf ("HELP: TGREP REGULAR_EXPR STRING.\n");
}

void run_test (char *reg, char *s, int count, ...)
{
	syntax_tree stree;
	va_list ap;
	vector<string> result;
	va_start (ap, count);
	while (count--) {
		result.push_back (va_arg (ap, char *));
	}
	va_end (ap);
	stree.build_tree (reg);
	dfa nfa;
	nfa.build_graph (stree.get_root ());
	matcher match (stree.get_root (), nfa.get_graph (), nfa.get_nstat ());
	match.match_string (s);

	if (match.match_test (result) == false) {
		cout << "Compare failed: Reg is: " << reg << " Str is :" << s << endl;
		stree.print_syntax_tree ();
		nfa.print_graph ();
		match.print_result ();
		exit (1);
	}
	cout << "Test OK." << endl;
}

void test_star ()
{
	cout << "Test for star." << endl;
	run_test ("abc", "abc", 1, "abc");
	run_test ("ab*c", "abbbc", 1, "abbbc");
	run_test ("***abc", "abc", 1, "abc");
	run_test ("*abc", "abc", 1, "abc");
	run_test ("a*bc", "abc", 1, "abc");
	run_test ("ab*c", "abc", 1, "abc");
	run_test ("a*b*c", "abc", 1, "abc");
	run_test ("a*b*c*", "abc", 1, "abc");
	run_test ("a**bc", "abc", 1, "abc");
	run_test ("abc**", "abc", 1, "abc");
	cout << "Star test OK." << endl;
}

void test_bracket ()
{
	cout << "Bracket test start" << endl;
	run_test ("(a)bc", "abc", 1, "abc");
	run_test ("(ab)c", "abc", 1, "abc");
	run_test ("(abc)", "abc", 1, "abc");
	run_test ("a(b)c", "abc", 1, "abc");
	run_test ("a(bc)", "abc", 1, "abc");
	run_test ("ab(c)", "abc", 1, "abc");
	run_test ("abc()", "abc", 1, "abc");
	run_test ("a(b*c)", "abc", 1, "abc");
	run_test ("()a(b*c)", "abc", 1, "abc");
	run_test ("()()abc", "abc", 1, "abc");
	run_test ("()abc()", "abc", 1, "abc");
	run_test ("()abc()()()", "abc", 1, "abc");
	cout << "Bracket test OK" << endl;
}

void test_or ()
{
	cout << "Or test start" << endl;
	run_test ("a|bc", "abc", 2, "a", "bc");
	run_test ("a|b|c", "abc", 3, "a", "b", "c");
	run_test ("ab|c", "abc", 2, "ab", "c");
	run_test ("|a|bc", "abc", 2, "a", "bc");
	run_test ("a|bc|", "abc", 2, "a", "bc");
	run_test ("a||bc", "abc", 2, "a", "bc");
	run_test ("||abc", "abc", 1, "abc");
	run_test ("||abc||", "abc", 1, "abc");
	cout << "Or test OK" << endl;
}

void test_mix_expr ()
{
	run_test ("(a*|b)c", "aaacbc", 2, "aaac", "bc");
}

void test_error_exit ()
{
	run_test ("(a)|*bc", "abc", 2, "a", "bc");
	run_test ("*", "abc", 0, "a");
}

void testsuite ()
{
	test_star ();
	test_bracket ();
	test_or ();
	test_mix_expr ();
}

int main (int argc, char **argv)
{
	/* Test mode.  */
	if (argc == 1) {
		testsuite ();
		return 0;
	}
	/* Execute mode.  */
	if (argc <= 2) {
		print_help ();
		exit (1);
	}
	syntax_tree stree;
	stree.build_tree (argv[1]);
	Stat nfa;
	nfa.build_graph_nfa (stree.get_root ());
#ifdef DEBUG
	nfa.print_graph ();
	stree.print_syntax_tree ();
#endif
	matcher match (stree.get_root (), nfa.get_graph (), nfa.get_nstat ());
	match.match_string (argv[2]);
	match.print_result ();
	return 0;
}

