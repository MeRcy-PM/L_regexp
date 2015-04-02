#include "stack.hpp"
#include "stree.hpp"
#include "dfa.hpp"
#include "matcher.hpp"

int main (int argc, char **argv)
{
	syntax_tree stree;
	stree.build_tree (argv[1]);
	dfa nfa;
	nfa.build_graph (stree.get_root ());
#ifdef DEBUG
	nfa.print_graph ();
	stree.print_syntax_tree ();
#endif
	matcher match (stree.get_root (), nfa.get_graph (), nfa.get_nstat ());
	match.match_string (argv[2]);
	match.print_result ();
	return 0;
}

