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
	nfa.print_graph ();
	stree.print_syntax_tree ();
	matcher match;
	match.match_string (nfa., argv[2]);
	return 0;
}

