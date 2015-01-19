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
