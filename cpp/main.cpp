#include "stack.hpp"

int main (int argc, char **argv)
{
	ops<int> op_stack;
	op_stack.print_ops ();
	ops<char> sym_stack;
	sym_stack.print_ops ();
	return 0;
}

