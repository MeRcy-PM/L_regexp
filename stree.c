#include "stree.h"
#include "stack.h"
stree_p syntax_tree = NULL;
void stree_init ()
{
	stack_stree_init ();
	graph_init ();
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
		printf ("AND");
		break;
	case NODE_OR:
		printf ("OR");
		break;
	case NODE_STAR:
		printf ("STAR");
		break;
	case NODE_ENTITY:
		printf ("ENTITY (%c)", node->id);
		break;
	case NODE_BRACKET:
		printf ("BRACKET");
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
	if (node->type == NODE_ENTITY)
		printf (" Index:%d", node->sindex);
	printf ("  na %s", node->nullable ? "true" : "false");
	print_set ("  first", node->first_op);
	print_set ("  last", node->last_op);
	printf ("\n");
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
		node->vertex = build_vertex_node (node->sindex, node);
		node->nullable = false;
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
		|| (entity_p (c) && ca == '*')
		|| (entity_p (c) && ca == ')')
		|| (ca == '*' && c == '('))
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

void stree_free_node (stree_p stree)
{
	if (stree == NULL)
		return;

	stree_free_node (stree->lchild);
	stree_free_node (stree->rchild);
	free (stree);
}

void stree_finit ()
{
	stree_free_node (syntax_tree);
}

set_p new_set_node ()
{
	set_p new = (set_p)malloc (sizeof (struct set));
	new->next = NULL;
	new->value = -1;
	return new;
}

bool in_set_p (set_p s, int value)
{
	while (s) {
		if (s->value == value)
			return true;
		s = s->next;
	}
	return false;
}

set_p set_union (set_p s1, set_p s2)
{
	set_p new = new_set_node ();
	set_p tail = new;
	set_p tmp = s1;
	/* Copy s1.  */
	while (1) {
		tail->value = tmp->value;
		if (tmp->next) {
			tail->next = new_set_node ();
			tail = tail->next;
			tmp = tmp->next;
		}
		else 
			break;
	}

	tmp = s2;
	/* Union s2.  */
	while (1) {
		if (tmp == NULL)
			break;

		if (!in_set_p (s1, tmp->value)) {
			tail->next = new_set_node ();
			tail = tail->next;
			tail->value = tmp->value;
		}
		tmp = tmp->next;
	}
	return new;
}

void print_set (char *set_name, set_p set)
{
	printf ("%s :", set_name);
	while (set) {
		printf ("%d ", set->value);
		set = set->next;
	}
}

set_p set_copy (set_p set)
{
	if (set == NULL)
		return NULL;
	set_p head;
	set_p new = new_set_node ();
	new->value = set->value;
	head = new;
	set = set->next;
	while (set) {
		new->next = new_set_node ();
		new = new->next;
		new->value = set->value;
		set = set->next;
	}
	return head;
}

