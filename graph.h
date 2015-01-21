#ifndef __GRAPH_H__
#define __GRAPH_H__
#include "stree.h"

typedef struct vertex* vertex_p;
struct vertex {
	int status;
	struct stree_node* stree;
	struct vertex* next;
	struct edge* edge;
};

struct edge {
	struct edge* next;
	int src;
	int dest;
	struct stree_node* trans;
};

struct graph {
	int n_vertex;
	struct vertex** vertex_pp;
};

extern struct graph sgraph;
extern vertex_p start_s;
extern vertex_p cur_s;
extern unsigned int status;

extern vertex_p build_vertex_node (int, struct stree_node*);
extern void graph_init (void);
extern void print_graph (void);
extern void build_graph (void);
extern void free_graph (void);
extern void free_vertex (void);
#define GET_STREE(s) (sgraph.vertex_pp[(s)]->stree)
#endif
