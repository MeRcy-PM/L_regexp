#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "base.h"
struct stree_node;
typedef struct stree_node* stree_p;
enum tree_type;

void print_id (struct stree_node *stree) {
	switch (stree->type) {
	case NODE_CAT:
		cout << "CAT";
		break;
	case NODE_ENTITY:
		cout << "ENTITY (" << (char)stree->id << ")";
		break;
	case NODE_OR:
		cout << "OR";
		break;
	case NODE_STAR:
		cout << "STAR";
		break;
	case NODE_BRACKET:
		cout << "BRACKET";
		break;
	default:
		assert (0);
		break;
	}
}

void print_vector (vector<bool> *v) {
	unsigned int i = 0;
	for (i = 0; i < v->size (); i++) {
		if (v->at(i))
			cout << i << " ";
	}
}

void print_stree (int deep, struct stree_node *stree) {
	if (stree->lchild)
		print_stree (deep + 1, stree->lchild);
	for (int i = 0; i < deep; i++)
		cout << "\t";
	//cout << "Index:" << sindex << "\t";
	print_id (stree);
	cout << " na: ";
	if	(stree->nullable) 
		cout << "true";
	else
		cout << "false";
	cout << "\t first_op: ";
	if (stree->first_op) print_vector (stree->first_op);
	cout << "\t last_op: ";
	if (stree->last_op) print_vector (stree->last_op);
	cout << endl;
	if (stree->rchild)
		print_stree (deep + 1, stree->rchild);
}

#endif
