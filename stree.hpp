#ifndef __STREE_H__
#define __STREE_H__
#include "base.h"
#include "stack.hpp"
enum tree_type {
    NODE_INIT = 0,
    NODE_CAT,
    NODE_OR,
    NODE_STAR,
    NODE_BRACKET,
    NODE_ENTITY,
    NODE_TOTAL
};

const int priority[NODE_TOTAL] = {0, 2, 1, 3, 0, 0};
unsigned status;
#define GET_PRIORITY(stree) (priority[(stree)->type])
#define CAT 257
class stree_node {
public:
	stree_node () : lchild (NULL),
				    rchild (NULL),
					type (NODE_INIT),
					nullable (true),
					first_op (NULL),
					last_op (NULL),
					sindex (0xffffffff),
					id (0xffff) {};
	~stree_node () {
		if (lchild) {
			delete lchild;
			lchild = NULL;
		}
		if (rchild) {
			delete rchild;
			rchild = NULL;
		}
		if (first_op)
			delete first_op;
		if (last_op)
			delete last_op;
	}
	void set_lchild (stree_node* stree) {lchild = stree;}
	void set_rchild (stree_node* stree) {rchild = stree;}
	void set_nullable (bool t) {nullable = t;}
	void set_id (unsigned short cid) {
		id = cid;
		if (id == CAT)
			type = NODE_CAT;
		else if (id == '|')
			type = NODE_OR;
		else if (id == '*')
			type = NODE_STAR;
		else if (id == '(' || id == ')')
			type = NODE_BRACKET;
		else if (id <= 255) {
			type = NODE_ENTITY;
			nullable = false;
			//Vertex
			sindex = status++;
		}
	}
	void print (int deep) {
		if (lchild)
			lchild->print (deep + 1);
		for (int i = 0; i < deep; i++)
			cout << "\t";
		//cout << "Index:" << sindex << "\t";
		this->print_id ();
		cout << " na: ";
		if  (nullable) 
			cout << "true";
		else
			cout << "false";
		cout << "\t first_op: ";
		print_vector (first_op);
		cout << "\t last_op: ";
		print_vector (last_op);
		cout << endl;
		if (rchild)
			rchild->print (deep + 1);
	}
	/* To judge whether the stack need to adjust.  */
	bool operator>= (stree_node *stree) {
		if (id == '(')
			return false;

		if (priority[type] <= GET_PRIORITY (stree))
			return true;

		return false;
	}
	stree_node* lchild;
    stree_node* rchild;
    enum tree_type type;
    bool nullable;
    vector<bool> *first_op;
    vector<bool> *last_op;
    unsigned int sindex;
    //struct vertex* vertex;
    unsigned short id;
private:
	void print_vector (vector<bool> *v) {
		unsigned int i = 0;
		for (i = 0; i < v->size (); i++) {
			if (v->at(i))
				cout << i << " ";
		}
	}
	void print_id () {
		switch (type) {
		case NODE_CAT:
			cout << "CAT";
			break;
		case NODE_ENTITY:
			cout << "ENTITY (" << (char)id << ")";
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
};

typedef stree_node* stree_p;
class syntax_tree {
public:
	syntax_tree () {status = 1;}
	~syntax_tree () {delete root;}
	void build_tree (char *s) {root = build_tree_1 (s);}
	void print_syntax_tree () {root->print (0);}
	stree_p get_root () {return root;}
private:
	void push_stack (stree_p stree) {
		if (stree->type == NODE_ENTITY)
			sym_stack.push (stree);
		else if (op_stack.is_need_adjust (stree))
			adjust_stack (stree);
		else
			op_stack.push (stree);
	}
	stree_p adjust_stack_1 (int proi, int last) {
    	if (op_stack.is_empty ())
        	if (!sym_stack.is_empty ())
            	return sym_stack.pop ();

    	if ((!last)
        	&& proi >= GET_PRIORITY (op_stack.curr ()))
        	return sym_stack.pop ();

    	stree_p ret = op_stack.pop ();
    	if (ret->type != NODE_STAR) {
        	ret->set_rchild (sym_stack.pop ());
        	ret->set_lchild (adjust_stack_1 (proi, last));
    	}
    	else {
        	/* Integrate the STAR EXPR as a symbol then continue.  */
        	ret->set_lchild (sym_stack.pop ());
			sym_stack.push (ret);
        	return adjust_stack_1 (proi, last);
    	}
    	return ret;
	}
	void adjust_stack (stree_p stree) {
    	stree_p adj;
    	/* For free memory of stree element ().  */
    	stree_p tmp = NULL;
    	if (stree == NULL) {
       		/* First of all, reduce to priority level 1.  */
        	adj = adjust_stack_1 (1, 0);
			sym_stack.push (adj);
        	/* Then pop all result.  */
        	adj = adjust_stack_1 (0, 1);
			sym_stack.push (adj);
    	}   
    	else {
			for (int i = 3; i > GET_PRIORITY (stree); i--) {
				adj = adjust_stack_1 (i, 0);
				sym_stack.push (adj);
			}
			adj = adjust_stack_1 (GET_PRIORITY (stree), 0);
        	/* Eliminate '()'.  */
        	if (stree->type == NODE_BRACKET) {
            	/* '(' will not adjust stack.  */
				tmp = op_stack.pop ();
				delete tmp;
				sym_stack.push (adj);
				delete stree;
        	}   
        	else {
				sym_stack.push (adj);
				op_stack.push (stree);
        	}   
    	}   
	}
	stree_p build_tree_1 (char *s) {
		if (s == NULL || s == '\0')
			return NULL;

		if (*s == ')') {
			cout << "Error Input." << endl;
			exit (1);
		}
		bool discat = false;
		char *fw = s + 1;
		char back = *s;
		stree_p arg = new stree_node ();
		arg->set_id (*s);
		push_stack (arg);
		while (*fw != '\0') {
			if (*fw == '|' || *fw == '*') {
				if (back == '('
					|| *fw == back) {
					fw++;
					continue;
				}
			}
			/* In case of '()'. Pop '('.  */
			if (*fw == ')' && back == '(') {
				op_stack.pop ();
				/* if 'a CAT (', need pop CAT node.  */
				if ((op_stack.curr ())->type == NODE_CAT)
					op_stack.pop ();
				back = *fw;
				fw++;
				continue;
			}
			if (need_cat_p (*fw, back, &discat)) {
				arg = new stree_node ();
				arg->set_id (CAT);
				push_stack (arg);
			}
			arg = new stree_node ();
			arg->set_id (*fw);
			push_stack (arg);
			back = *fw;
			fw++;
		}
		adjust_stack (NULL);
		return sym_stack.pop ();
	}
	bool entity_p (char c) {
    	if (c != '|' && c != '(' && c != ')' && c != '*')
        	return true;
    	return false;
	}
	bool need_cat_p (char c, char ca, bool *discat) {
		if (*discat) {
			*discat = false;
			return false;
		}
			
  	  	if ((entity_p (c) && entity_p (ca))
 	       || (entity_p (ca) && c == '(')
 	       || (entity_p (c) && ca == '*')
    	   || (entity_p (c) && ca == ')')
           || (ca == '*' && c == '(')
		   || (ca == ')' && c == '('))
          return true;
      	return false;
	}
	ops<stree_p> op_stack;
	syms<stree_p> sym_stack;
	stree_p root;
};

#endif
