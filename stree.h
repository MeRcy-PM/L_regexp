#ifndef __STREE_H__
#define __STREE_H__
#include "base.h"
#include "graph.h"
enum tree_type {
	NODE_INIT = 0,
	NODE_CAT,
	NODE_OR,
	NODE_STAR,
	NODE_BRACKET,
	NODE_ENTITY,
	NODE_TOTAL
};

struct set {
	struct set* next;
	int value;
};

typedef struct set* set_p;

struct stree_node {
	struct stree_node* lchild;
	struct stree_node* rchild;
	enum tree_type type;
	bool nullable;
	set_p first_op;
	set_p last_op;
	unsigned int sindex;
	struct vertex* vertex;
	unsigned short id;
};

typedef struct stree_node* stree_p;
stree_p syntax_tree;
int priority[NODE_TOTAL];
extern void print_stree (stree_p);
extern stree_p build_stree_node (short);
extern void build_syntax_tree (char *);
extern bool entity_p (char);
extern void stree_finit (void);

extern set_p set_union (set_p, set_p);
extern bool in_set_p (set_p, int);
extern set_p new_set_node (void);
extern void print_set (char *, set_p);
extern set_p set_copy (set_p);
extern void add_edge (struct set*, struct set*);
extern void free_set (set_p);
#define GET_PRIORITY(s) (priority[(s)->type])
#endif
