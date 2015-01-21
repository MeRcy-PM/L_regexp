#include "dfa.h"

static void compute_property_1 (stree_p stree)
{
	if (stree == NULL)
		return;

	switch (stree->type) {
	case NODE_CAT:
		compute_property_1 (stree->lchild);
		compute_property_1 (stree->rchild);
		stree->nullable = stree->lchild->nullable & stree->rchild->nullable;
		stree->first_op = stree->lchild->nullable ?
			set_union (stree->lchild->first_op, stree->rchild->first_op):
			set_copy (stree->lchild->first_op);
		stree->last_op = stree->rchild->nullable ?
			set_union (stree->lchild->last_op, stree->rchild->last_op):
			set_copy (stree->rchild->last_op);
		return;
		break;
	case NODE_OR:
		compute_property_1 (stree->lchild);
		compute_property_1 (stree->rchild);
		stree->nullable = stree->lchild->nullable | stree->rchild->nullable;
		stree->first_op = set_union (stree->lchild->first_op, stree->rchild->first_op);
		stree->last_op = set_union (stree->lchild->last_op, stree->rchild->last_op);
		return;
		break;
	case NODE_STAR:
		compute_property_1 (stree->lchild);
		stree->nullable = true;
		stree->first_op = set_copy (stree->lchild->first_op);
		stree->last_op = set_copy (stree->lchild->last_op);
		return;
		break;
	case NODE_ENTITY:
		/* Leaf. Have null entity?  */
		stree->nullable = false;
		stree->first_op = new_set_node ();
		stree->first_op->value = stree->sindex;
		stree->last_op = new_set_node ();
		stree->last_op->value = stree->sindex;
		return;
		break;
	default:
		printf ("Error Tree Type Detected.\n");
		exit (1);
	}
	return;
}

void compute_property ()
{
	
	compute_property_1 (syntax_tree);
	return;
}

void compute_follow_op (stree_p stree)
{
	if (stree == NULL)
		return;
	
	/* P112.  */
	if (stree->type == NODE_STAR)
		add_edge (stree->last_op, stree->first_op);

	if (stree->type == NODE_CAT)
		add_edge (stree->lchild->last_op, stree->rchild->first_op);
	
	compute_follow_op (stree->lchild);
	compute_follow_op (stree->rchild);
	return;
}

void compute_nfa_trans ()
{
	set_p set_zero = new_set_node ();
	set_zero->value = 0;
	add_edge (set_zero, syntax_tree->first_op);
	free_set (set_zero);
	compute_follow_op (syntax_tree);
}
