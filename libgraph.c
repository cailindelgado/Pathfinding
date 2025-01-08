#include "libgraph.h"

extern void draw_pos(int x,int y);

static void
delete_item (void *data)
{
    int *rv = (int *)data;
    if (rv) {
        free (rv);
    }
    return;
}

int
cmp_route(const void* a,const void* b)
{
    route_t* ra = (route_t*)a;
    route_t* rb = (route_t*)b;
    return ra->start - rb->start;
}

graph_t*
graph_new()
{
    graph_t *graph;
    graph = safe_malloc (sizeof (graph_t));
    graph->order = 0;
    graph->width = 0;
    graph->height = 0;
    graph->graph = NULL;
    graph->visited = NULL;
    graph->size = sizeof(graph_t);
    return (graph);
}

void
graph_delete(graph_t *graph)
{
    int i;
    assert (graph != NULL);
    assert (graph->graph != NULL);
    for (i = 0; i < graph->order; i++) {
        delete_list ((dlist_t *)graph->graph[i], delete_item);
    }
    free (graph->graph);
    if(graph->visited) delete_bitvector(graph->visited);
    free (graph);
    return;
}

static int
graph_read_edges (graph_t *graph, FILE *edges)
{
    int src;
    int dst;

    while (fscanf(edges, "%d - %d\n", &src, &dst) == 2) {
        if (src >= graph->order || src < 0) {
            fprintf(stderr, "Do not recognise vertex number %d.\n", src);
            return (EXIT_FAILURE);
        } else if (dst >= graph->order || dst < 0) {
            fprintf(stderr, "Do not recognise vertex number %d.\n", dst);
            return (EXIT_FAILURE);
        }
        int *val = safe_malloc (sizeof(int));
        *val = dst;
        insert_in_order ((dlist_t*)graph->graph[src],val,sizeof(int), cmp_int);
    }
    return (EXIT_SUCCESS);
}

void
graph_read(graph_t *graph, FILE *edges)
{
    int i;
    int order;
    int width;
    int height;
    size_t memusage = 0;

    if (fscanf(edges, "%d %d %d\n", &order,&width,&height) != 3) {
        fprintf(stderr, "ERROR: could not read graph order.\n");
        exit(EXIT_FAILURE);
    } else {
        assert(order >= 0);
        assert(width*height == order);
        graph->order = order;
        graph->width = width;
        graph->height = height;

        memusage = graph->order * sizeof(void *);
        graph->graph = safe_malloc (memusage);
        graph->size = memusage;
        for (i = 0; i < graph->order; i++) {
            graph->graph[i] = (void*) make_empty_list ();
        }

        memusage = graph->order * sizeof(int);
        graph->visited = new_bitvector(graph->order);
        graph->size += sizeof(bitvector_t);
        graph->size += WORD_COUNT (order) * sizeof (uint32_t);

        if (graph_read_edges(graph, edges) != 0) {
            fprintf(stderr, "ERROR: could not read graph edges.\n");
            exit(EXIT_FAILURE);
        }

        /* If we use link lists, we must check the final sizes of each now */
        for (i = 0; i < graph->order; i++) {
            dlist_t * tl = (dlist_t *) graph->graph[i];
            graph->size += tl->size;
        }
    }
}

void
graph_print(graph_t *graph)
{
    int i;

    assert(graph != NULL);
    assert(graph->graph != NULL);

    for (i = 0; i < graph->order; i++) {
        dlist_t *tlist = (dlist_t *)graph->graph[i];
        dlnode_t *tnode = tlist->head;
        fprintf(stdout, "%d: ", i);
        while (tnode != NULL) {
            fprintf(stdout, "-> %d ", *((int *)tnode->data));
            tnode = tnode->next;
        }
        fprintf(stdout, "\n");
    }
    return;
}

void
graph_visit(graph_t* graph,int id)
{
    /* mark as part of the maze */
    set_bit(graph->visited,id);

    /* draw */
    /* calc x and y 
    int x = id % graph->width;
    int y = id / graph->width;
    */

    /* TODO: commented out to get test_path to build*/
    /*  draw_pos(x,y); */
}


