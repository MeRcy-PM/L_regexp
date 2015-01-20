#include "graph.h"

struct graph sgraph;
vertex_p start_s;
vertex_p cur_s;
unsigned int status = 1;

vertex_p new_vertex_node (int index, stree_p stree)
{
	vertex_p new = (vertex_p)malloc (sizeof (struct vertex));
	new->status = index;
	new->stree = stree;
	new->next = NULL;
	new->edge = NULL;
	return new;
}

void graph_init ()
{
	start_s = cur_s = new_vertex_node (0, NULL);
}

vertex_p build_vertex_node (int index, struct stree_node* stree)
{
	vertex_p new = new_vertex_node (index, stree);
	cur_s->next = new;
	cur_s = cur_s->next;
	return new;
}

void print_graph ()
{
	int i;
	printf ("--------------------PRINT GRAPH------------------\n");
	printf ("Total Vertex number is %d\n", sgraph.n_vertex);
	printf ("INDEX 0: Start Status.\n");
	for (i = 1; i < sgraph.n_vertex; i++) {
		printf ("INDEX %d: element is %c\n", i, sgraph.vertex_pp[i]->stree->id);
	}
}

void build_graph ()
{
	vertex_p tmp = start_s;
	int index = 0;
	sgraph.n_vertex = status;
	sgraph.vertex_pp = (struct vertex**)malloc (status * sizeof (struct vertex*));
	while (tmp) {
		sgraph.vertex_pp[index++] = tmp;
		tmp = tmp->next;
	}
}
