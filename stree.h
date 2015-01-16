#ifndef __STREE__
#define __STREE__
#include "base.h"
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
	unsigned short id;
};

typedef struct stree_node* stree_p;
stree_p syntax_tree;
int priority[NODE_TOTAL];
extern void print_stree (stree_p);
extern stree_p build_stree_node (short);
extern void build_syntax_tree (char *);
extern bool entity_p (char);
#define GET_STATUS(s) ((s)->sindex.sindex)
#define GET_PRIORITY(s) (priority[(s)->type])
#endif