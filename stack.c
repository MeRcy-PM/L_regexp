#include "stack.h"
#include "stree.h"

bool stack_empty_p (struct stack_stree stack)
{
	if (stack.index == 1)
		return true;
	return false;
}

static inline void push_stack (stree_p stree, struct stack_stree *stack)
{
#ifdef __DEBUG__
	printf ("PUSH -----------------------------\n");
	print_stree (stree);
#endif
	stack->stack[stack->index++] = stree;
	return;
}

stree_p pop_stack (struct stack_stree* stack)
{
	// check_stack_pop (stack);
	int index = --(stack->index);
	stree_p ret = stack->stack[index];
#ifdef __DEBUG__
	printf ("POP--------------------------------\n");
	print_stree (ret);
#endif
	return ret;
}

static stree_p adjust_stack_1 (int proi, int last)
{
	if (stack_empty_p (op_stack))
		if (!stack_empty_p (sym_stack))
			return pop_stack (&sym_stack);

	if ((!last)
	    && proi >= GET_PRIORITY (STACK_CUR (op_stack)))
		return pop_stack (&sym_stack);

	stree_p ret = pop_stack (&op_stack);
	if (ret->type != NODE_STAR) {
		ret->rchild = pop_stack (&sym_stack);
		ret->lchild = adjust_stack_1 (proi, last);
	}
	else {
		/* Integrate the STAR EXPR as a symbol then continue.  */
		ret->lchild = pop_stack (&sym_stack);
		push_stack (ret, &sym_stack);
		return adjust_stack_1 (proi, last);
	}
	return ret;
}

void adjust_stack (stree_p stree)
{
	stree_p adj;
	if (stree == NULL) {
		/* First of all, reduce to priority level 1.  */
		adj = adjust_stack_1 (1, 0);
		push_stack (adj, &sym_stack);
		/* Then pop all result.  */
		adj = adjust_stack_1 (0, 1);
		push_stack (adj, &sym_stack);
	}
	else {
		adj = adjust_stack_1 (GET_PRIORITY (stree), 0);
		/* Eliminate '()'.  */
		if (stree->type == NODE_BRACKET) {
			pop_stack (&op_stack);
			push_stack (adj, &sym_stack);
		}
		else {
			push_stack (adj, &sym_stack);
			push_stack (stree, &op_stack);
		}
	}
}

/* Return true if current priority is less than stack.  */
/* Rules:
 * 1. '(' do not need to adjust stack.
 * 2. ')' have the lowest priority (0) in order to adjust stack when we meet it.
 * 3. Current priority lower than stack's, adjust it.  */
static inline bool need_adjust_stack_p (stree_p stree, struct stack_stree stack)
{
	if (stack_empty_p (op_stack))
		return false;

	if (stree->id == '(')
		return false;

	if (GET_PRIORITY (stree) <= GET_PRIORITY (STACK_CUR (stack)))
		return true;

	return false;
}

static inline void push_stack_op (stree_p stree)
{
	//check_stack (stack);
	if (need_adjust_stack_p (stree, op_stack)) {
		adjust_stack (stree);
	}
	else
		push_stack (stree, &op_stack);
	return;
}

void push_stack_stree (stree_p stree)
{
	if (stree->type == NODE_ENTITY)
		push_stack (stree, &sym_stack);
	else 
		push_stack_op (stree);

	return;
}

void stack_stree_init ()
{
	sym_stack.index = 1;
	op_stack.index = 1;
	return;
}
