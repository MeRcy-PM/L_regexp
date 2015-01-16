#ifndef __STACK_FOR_STREE__
#define __STACK_FOR_STREE__
#include "stree.h"
#include "base.h"
//enum stack_type {
//	TYPE_INIT = 0,
//	TYPE_SYM,
//	TYPE_OP
//};

struct stack_stree {
	stree_p stack[100];
	int index;
};

struct stack_stree sym_stack;
struct stack_stree op_stack;

extern void stack_stree_init (void);
extern stree_p pop_stack (struct stack_stree *);
extern void adjust_stack (stree_p);
extern void push_stack_stree (stree_p);
#define STACK_CUR(s) ((s).stack[(s).index - 1])
#endif
