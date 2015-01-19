#include "dfa.h"

static void compute_nullable_1 (stree_p stree)
{
	if (stree == NULL)
		return;

	switch (stree->type) {
	case NODE_CAT:
		compute_nullable_1 (stree->lchild);
		compute_nullable_1 (stree->rchild);
		stree->nullable = stree->lchild->nullable & stree->rchild->nullable;
		return;
		break;
	case NODE_OR:
		compute_nullable_1 (stree->lchild);
		compute_nullable_1 (stree->rchild);
		stree->nullable = stree->lchild->nullable | stree->rchild->nullable;
		return;
		break;
	case NODE_STAR:
		compute_nullable_1 (stree->lchild);
		stree->nullable = true;
		return;
		break;
	case NODE_ENTITY:
		/* Leaf. Have null entity?  */
		stree->nullable = false;
		return;
		break;
	default:
		printf ("Error Tree Type Detected.\n");
		exit (1);
	}
	return;
}

void compute_nullable ()
{
	
	compute_nullable_1 (syntax_tree);
	return;
}

static void compute_first_op_1 (stree_p stree)
{
	if (stree == NULL)
		return;

	switch (stree->type) {
	case NODE_CAT:
		compute_first_op_1 (stree->lchild);
		compute_first_op_1 (stree->rchild);
		stree->first_op = stree->lchild->nullable ?
			set_union (stree->lchild->first_op, stree->rchild->first_op):
			set_copy (stree->lchild->first_op);
		return;
		break;
	case NODE_OR:
		compute_first_op_1 (stree->lchild);
		compute_first_op_1 (stree->rchild);
		stree->first_op = set_union (stree->lchild->first_op, stree->rchild->first_op);
		return;
		break;
	case NODE_STAR:
		compute_first_op_1 (stree->lchild);
		stree->first_op = set_copy (stree->lchild->first_op);
		return;
		break;
	case NODE_ENTITY:
		/* Leaf. Have null entity?  */
		stree->first_op = new_set_node ();
		stree->first_op->value = stree->sindex;
		return;
		break;
	default:
		printf ("Error Tree Type Detected.\n");
		exit (1);
	}
}

void compute_first_op ()
{
	compute_first_op_1 (syntax_tree);
	return;
}


static void compute_last_op_1 (stree_p stree)
{
	if (stree == NULL)
		return;

	switch (stree->type) {
	case NODE_CAT:
		compute_last_op_1 (stree->lchild);
		compute_last_op_1 (stree->rchild);
		stree->last_op = stree->rchild->nullable ?
			set_union (stree->lchild->last_op, stree->rchild->last_op):
			set_copy (stree->rchild->last_op);
		return;
		break;
	case NODE_OR:
		compute_last_op_1 (stree->lchild);
		compute_last_op_1 (stree->rchild);
		stree->last_op = set_union (stree->lchild->last_op, stree->rchild->last_op);
		return;
		break;
	case NODE_STAR:
		compute_last_op_1 (stree->lchild);
		stree->last_op = set_copy (stree->lchild->last_op);
		return;
		break;
	case NODE_ENTITY:
		/* Leaf. Have null entity?  */
		stree->last_op = new_set_node ();
		stree->last_op->value = stree->sindex;
		return;
		break;
	default:
		printf ("Error Tree Type Detected.\n");
		exit (1);
	}
}

void compute_last_op ()
{
	compute_last_op_1 (syntax_tree);
	return;
}
