#include "stree.h"
#include "stack.h"
stree_p syntax_tree = NULL;
unsigned int status = 0;
void stree_init ()
{
	stack_stree_init ();	
	memset (&priority, 0, NODE_TOTAL);
	priority[NODE_CAT] = 1;
	priority[NODE_OR] = 1;
	priority[NODE_STAR] = 2;
}

static void inline print_table (int deep)
{
	int i;
	for (i = 0; i < deep; i++)
		printf ("\t");
	return;
}

static void print_id (stree_p node)
{
	switch (node->type) {
	case NODE_CAT:
		printf ("AND\n");
		break;
	case NODE_OR:
		printf ("OR\n");
		break;
	case NODE_STAR:
		printf ("STAR\n");
		break;
	case NODE_ENTITY:
		printf ("ENTITY (%c)\n", node->id);
		break;
	case NODE_BRACKET:
		printf ("BRACKET\n");
		break;
	default:
		break;
	}
	return;
}

void print_stree_1 (stree_p node, int deep)
{
	if (node == NULL)
		return;
	print_stree_1 (node->lchild, deep + 1);
	print_table (deep);
	print_id (node);
	print_stree_1 (node->rchild, deep + 1);
	return;
}

void print_stree (stree_p node)
{
	if (node == NULL)
		print_stree_1 (syntax_tree, 0);
	else
		print_stree_1 (node, 0);
	return;
}

stree_p new_node ()
{
	stree_p node = (stree_p)malloc (sizeof (struct stree_node));
	if (node == NULL) {
		printf ("Malloc fail.\n");
		exit (1);
	}

	node->lchild = NULL;
	node->rchild = NULL;
	node->type = NODE_INIT;
	node->nullable = true;
	node->first_op = NULL;
	node->last_op = NULL;
	node->sindex = 0xffffffff;
	node->id = 0xffff;
	return node;
}

stree_p build_stree_node (short id)
{
	stree_p node = new_node ();
	if (id == '|') {
		node->type = NODE_OR;
		node->id = id;
	}
	else if (id == '*') {
		node->type = NODE_STAR;
		node->id = id;
	}
	else if (id == CAT) {
		node->type = NODE_CAT;
		node->id = id;
	}
	else if (id == '(' || id == ')') {
		node->type = NODE_BRACKET;
		node->id = id;
	}
	else {
		node->type = NODE_ENTITY,
		node->id = id;
		node->sindex = status++;
	}
	return node;
}

bool entity_p (char c)
{
	if (c != '|' && c != '(' && c != ')' && c != '*')
		return true;
	return false;
}

bool need_cat_p (char c, char ca)
{
	if ((entity_p (c) && entity_p (ca))
		|| (entity_p (ca) && c == '(')
		|| (entity_p (c) && ca == '*'))
		return true;
	return false;
}

stree_p build_syntax_tree_1 (char *s, bool subset)
{
	if (s == NULL || *s == '\0')
		return NULL;
	char *fw = s + 1;
	stree_p arg;
	arg = build_stree_node (*s);
	push_stack_stree (arg);
	while (*fw != '\0') {
		if (need_cat_p (*fw, *(fw- 1))) {
			arg = build_stree_node (CAT);
			push_stack_stree (arg);
		}
		arg = build_stree_node (*fw);
		push_stack_stree (arg);
		fw++;
	}
	adjust_stack (NULL);
	return pop_stack (&sym_stack);
}

void build_syntax_tree (char *s)
{
	stree_init ();
	syntax_tree = build_syntax_tree_1 (s, false);
	return;
}
