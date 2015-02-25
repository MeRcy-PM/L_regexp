#include "stack.hpp"

int main (int argc, char **argv)
{
	ops<int> op_stack;
	op_stack.push (1);
	op_stack.push (2);
	op_stack.print_stack ();
	syms<char> sym_stack;
	sym_stack.push ('a');
	sym_stack.print_stack ();
	return 0;
}

