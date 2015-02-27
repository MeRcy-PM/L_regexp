#include "stack.hpp"
#include "stree.hpp"
#include "dfa.hpp"

int main (int argc, char **argv)
{
	syntax_tree stree;
	stree.build_tree (argv[1]);
	dfa nfa;
	nfa.build_graph (stree.get_root ());
	nfa.print_graph ();
	stree.print_syntax_tree ();
	return 0;
}

