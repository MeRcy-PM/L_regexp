#ifndef __DFA_H__
#define __DFA_H__
#include "base.h"
#include "stree.hpp"
struct vertex {
	char id;
	vector<bool>* edge;
};

class Stat {
public:
	Stat () {
		stat = new struct vertex[status];
		nstat = status;
		for (unsigned i = 0; i < nstat; i++)
			stat[i].edge = new vector<bool> (nstat, false);
	}
	~Stat () {
		for (unsigned i = 0; i < nstat; i++)
			delete stat[i].edge;
		if (stat != NULL) {
			delete [] stat;
			stat = NULL;
		}
	}
	void build_graph_nfa (stree_p stree) {
		compute_property (stree);
		compute_start_edge (stree);
		compute_edge (stree);
	}
	void build_graph_dfa (stree_p stree) {
	
	}
	void print_graph () {
		for (unsigned i = 0; i < nstat; i++)
			print_edge (i);
	}
	struct vertex* get_graph () {return stat;}
	unsigned get_nstat () {return nstat;}
private:
	void compute_start_edge (stree_p stree) {
		for (unsigned i = 0; i < nstat; i++)
			stat[0].edge->at (i) = stree->first_op->at (i);
	}
	void add_edge (vector<bool> *vsrc, vector<bool> *vdest) {
		if (vsrc->size () != vdest->size ()) {
			cout << "Set Error." << endl;
			exit (1);
		}
		unsigned size = vsrc->size ();
		for (unsigned i = 1; i < size; i++) {
			if (vsrc->at (i)) {
				for (unsigned j = 1; j < size; j++)
					stat[i].edge->at (j) = vdest->at (j) | stat[i].edge->at (j);
			}
		}
	}
	void compute_edge (stree_p stree) {
		if (stree == NULL)
			return;

		if (stree->type == NODE_STAR)
			add_edge (stree->last_op, stree->first_op);
		else if (stree->type == NODE_CAT)
			add_edge (stree->lchild->last_op, stree->rchild->first_op);

		compute_edge (stree->lchild);
		compute_edge (stree->rchild);
	}
	vector<bool>* set_union (vector<bool> *v1, vector<bool> *v2) {
		if (v1->size () != v2->size ()) {
			cout << "Set error." << endl;
			exit (1);
		}
		unsigned int size = v1->size ();
		vector<bool>* tmp = new vector<bool> (size);
		for (unsigned int i = 0; i < tmp->size (); i++) {
			tmp->at(i) = v1->at(i) | v2->at(i);
		}
		return tmp;
	}
	void compute_property (stree_p stree) {
    	if (stree == NULL)
        	return;

    	switch (stree->type) {
    	case NODE_CAT:
        	compute_property (stree->lchild);
	        compute_property (stree->rchild);
	        stree->nullable = stree->lchild->nullable & stree->rchild->nullable;
	        stree->first_op = stree->lchild->nullable ?
	            set_union (stree->lchild->first_op, stree->rchild->first_op):
	            new vector<bool> (*stree->lchild->first_op);
	        stree->last_op = stree->rchild->nullable ?
	            set_union (stree->lchild->last_op, stree->rchild->last_op):
	            new vector<bool> (*stree->rchild->last_op);
	        return;
	        break;
	    case NODE_OR:
	        compute_property (stree->lchild);
	        compute_property (stree->rchild);
	        stree->nullable = stree->lchild->nullable | stree->rchild->nullable;
	        stree->first_op = set_union (stree->lchild->first_op, stree->rchild->first_op);
	        stree->last_op = set_union (stree->lchild->last_op, stree->rchild->last_op);
	        return;
	        break;
	    case NODE_STAR:
	        compute_property (stree->lchild);
	        stree->nullable = true;
	        stree->first_op = new vector<bool> (*stree->lchild->first_op);
	        stree->last_op = new vector<bool> (*stree->lchild->last_op);
	        return;
	        break;
	    case NODE_ENTITY:
			/* Leaf. Have null entity?  */
	        stree->nullable = false;
	        stree->first_op = new vector<bool> (nstat, false);
	        stree->first_op->at (stree->sindex) = true;
	        stree->last_op = new vector<bool> (nstat, false);
	        stree->last_op->at (stree->sindex) = true;
			stat[stree->sindex].id = stree->id;
	        return;
	        break;
	    default:
	        cout << "Error Tree Type Detected." << endl;
	        exit (1);
	    }
    	return;
	}
	void print_edge (int index) {
		cout << "Vertex Index " << index << " can get the vertex index: " << endl;
		for (unsigned i = 0; i < stat[index].edge->size (); i++)
			if (stat[index].edge->at (i))
				cout << "\tIndex " << i << " Trans word " << stat[i].id << endl;
	}
	unsigned nstat;
	struct vertex* stat;
};
#endif
