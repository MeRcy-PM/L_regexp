#include "graph.h"

struct graph sgraph;
vertex_p start_s;
vertex_p cur_s;
unsigned int status;

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
	status = 1;
	start_s = cur_s = new_vertex_node (0, NULL);
}

vertex_p build_vertex_node (int index, struct stree_node* stree)
{
	vertex_p new = new_vertex_node (index, stree);
	cur_s->next = new;
	cur_s = cur_s->next;
	return new;
}

void print_edge (struct edge* edge)
{
	while (edge) {
		printf ("\tFrom Index %d to Index %d, Trans Key word is %c\n",
				edge->src, edge->dest, edge->trans->id);
		edge = edge->next;
	}
}

void print_graph ()
{
	int i;
	printf ("--------------------PRINT GRAPH------------------\n");
	printf ("Total Vertex number is %d\n", sgraph.n_vertex);
	printf ("INDEX 0: Start Status.\n");
	printf ("Print edge of INDEX 0\n");
	print_edge (sgraph.vertex_pp[0]->edge);
	for (i = 1; i < sgraph.n_vertex; i++) {
		printf ("INDEX %d: element is %c\n", i, sgraph.vertex_pp[i]->stree->id);
		printf ("Print edge of INDEX %d\n", i);
		print_edge (sgraph.vertex_pp[i]->edge);
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

void free_graph ()
{
	int i;
	for (i = 0; i < sgraph.n_vertex; i++)
		free_edge (sgraph.vertex_pp[i]->edge);
	free (sgraph.vertex_pp);
}

void free_vertex ()
{
	vertex_p tmp;
	while (start_s) {
		tmp = start_s;
		start_s = start_s->next;
		free (tmp);
	}
}

struct edge* new_edge (int src, int dest)
{
	struct edge* new = (struct edge*)malloc (sizeof (struct edge));
	new->src = src;
	new->dest = dest;
	new->next = NULL;
	new->trans = GET_STREE (dest);
	return new;
}

void add_edge_1 (int src, int dest)
{
	struct edge* tmp;
	/* If the edge is exist, no need to add.  */
	tmp = sgraph.vertex_pp[src]->edge;
	/* No edge, must add to stree.  */
	if (tmp == NULL) {
		sgraph.vertex_pp[src]->edge = new_edge (src, dest);
		return;
	}

	if (tmp->dest == dest)
		return;

	while (tmp->next) {
		if (tmp->next->dest == dest)
			return;
		tmp = tmp->next;
	}
	/* Not match edge is found.  */
	tmp->next = new_edge (src, dest);
}

void add_edge (struct set* src, struct set* dest)
{
	set_p dest_tmp;
	if (dest == NULL)
		return;

	while (src) {
		dest_tmp = dest;
		while (dest_tmp) {
			add_edge_1 (src->value, dest_tmp->value);
			dest_tmp = dest_tmp->next;
		}
		src = src->next;
	}
}

void free_edge (struct edge* edge)
{
	struct edge* tmp;
	while (edge) {
		tmp = edge;
		edge = edge->next;
		free (tmp);
	}
}
