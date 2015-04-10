#ifndef __STREE_H__
#define __STREE_H__
#include "base.h"
extern void print_stree (int, struct stree_node *);
enum tree_type {
    NODE_INIT = 0,
    NODE_CAT,
    NODE_OR,
    NODE_STAR,
    NODE_BRACKET,
    NODE_ENTITY,
    NODE_TOTAL
};

static const char *LEC_next = "nt\\()0\0";
static const char *LEC = "\n\t\\()\0\0";
unsigned status;
/* Means that unsupport this ESC.  */
#define ESC_FAIL 'F'
enum {CAT = 257, LBCK, RBCK, STAR, OR, TERM};
struct stree_node {
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
	stree_node* lchild;
    stree_node* rchild;
    enum tree_type type;
    bool nullable;
    vector<bool> *first_op;
    vector<bool> *last_op;
    unsigned int sindex;
    unsigned short id;
};

typedef struct stree_node* stree_p;
class syntax_tree {
public:
	syntax_tree (const char *s): current (s),
							  id (0xffff),
							  layer (0) {status = 1;}
	~syntax_tree () {delete root;}
	void build_tree () {root = expr ();}
	void print_syntax_tree () {print_stree (0, root);}
	stree_p get_root () {return root;}
private:
	void set_node_id (unsigned short cid, stree_p stree) {
		stree->id = cid;
		if (cid == CAT)
			stree->type = NODE_CAT;
		else if (cid == OR)
			stree->type = NODE_OR;
		else if (cid == STAR)
			stree->type = NODE_STAR;
		else if (cid == LBCK || cid == RBCK)
			stree->type = NODE_BRACKET;
		else if (cid <= 255) {
			stree->type = NODE_ENTITY;
			stree->nullable = false;
			stree->sindex = status++;
		}
	}
	stree_p inline build_node (unsigned short cid) {
		stree_p tmp = new stree_node;
		set_node_id (cid, tmp);
		return tmp;
	}
	void next () {
		char c = *current++;
		if (c == '\\') {
			if ((id = get_esc (*current)) == ESC_FAIL)
				id = c;
			else
				current++;
		}
		else if (c == '*') {
			id = STAR;
			while (*current == '*')
				current++;
		}
		else if (c == '|') {
			id = OR;
			while (*current == '|')
				current++;
			/* Or in last.  */
			if (*current == '\0')
				id = TERM;
			/* Or before ')'.  */
			else if (*current == ')') {
				id = RBCK;
				current++;
			}
		}
		else if (c == '(')
			id = LBCK;
		else if (c == ')')
			id = RBCK;
		else if (c == '\0')
			id = TERM;
		else id = c;
	}
	stree_p inline cat_expr (stree_p prev, stree_p next)
	{
		stree_p tmp = build_node (CAT);
		tmp->lchild = prev;
		tmp->rchild = next;
		return tmp;
	}
	stree_p expr () {
		stree_p stree = NULL, prev = NULL, tmp = NULL;
		/* Initialization.  */
		if (id == 0xffff)
			next ();
		/* Ignore start with or.  */
		if (id == OR)
			next ();
		if (id == STAR)
			ERROR ("Nothing to repeat.\n");
		if (id == TERM && layer != 0)
			ERROR ("Lost bracket.\n");
		if (id == RBCK) {
			if (layer == 0)
				ERROR ("Lost bracket.\n");
			layer--;
			return NULL;
		}
		/* This is the case of bracket in front of expression.
		    Building a tree for '('. is unexpected.  */
		if (id == LBCK) {
			next ();
			layer++;
			stree = prev = expr ();
			if (id != RBCK)
				ERROR ("Lost bracket.\n");
			next ();
			/* Skip the "()".  */
			if (stree == NULL)
				return expr ();
		}
		if (stree == NULL) {
			/* Generate stree.  */
			stree = prev = build_node (id);
			next ();
		}
		while (id != RBCK && id != TERM) {
			if (id == OR) {
				tmp = build_node (OR);
				tmp->lchild = prev;
				next ();
				tmp->rchild = expr ();
				stree = prev = tmp;
				/* Like "(b|c)".  */
				if (id == RBCK)
					return stree;
				continue;
			}
			if (id == LBCK) {
				layer++;
				next ();
				stree = expr ();
				next ();
				/* Skip "()".  */
				if (stree == NULL)
					stree = prev;
				else
					/* New expr must CAT to prev.  */
					stree = prev = cat_expr (prev, stree);
				continue;
			}
			else {
				stree = build_node (id);
				next ();
			}
			/* If ab*, a cat b, then see star,
			    need to add star node to the stree's rchild.  */
			if (stree->id == STAR) {
				if (prev->rchild) {
					stree->lchild = prev->rchild;
					prev->rchild = stree;
					stree = prev;
				}
				else {
					stree->lchild = prev;
					prev = stree;
				}
				continue;
			}
			stree = prev = cat_expr (prev, stree);
		}
		if (id == TERM && layer != 0)
			ERROR ("Lost bracket.\n");
		if (id == RBCK)
			layer--;
		return stree;
	}
	char get_esc (char c) const
	{
		int i = 0;
		while (LEC_next[i] != '\0') {
			if (c == LEC_next[i])
				return LEC[i];
			i++;
		}
		return ESC_FAIL;
	}
	stree_p root;
	/* Current for token.  */
	const char *current;
	/* May use struct token to descript format like '[A-Z]'.  */
	unsigned short id;
	int layer;
};

#endif
