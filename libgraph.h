/*** Utility Types ***/
#ifndef _LIBGRAPH_H_
#define _LIBGRAPH_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/

#include "libutil.h"
#include "liblist.h"
#include "libbits.h"


typedef struct
{
    int order;            /* Total distinct nodes */
    int width;            /* Width of the maze */
    int height;           /* Height of the maze */
    void **graph;  /* The core graph data structure */
    bitvector_t* visited; /* Used to track traversal */
    int size;             /* Size of the graph representation in bytes */
} graph_t;

typedef struct {
    int start;
    int stop;
} route_t;

int cmp_route(const void* a,const void* b);

graph_t* graph_new();
void     graph_delete(graph_t *graph);
void     graph_read(graph_t *graph, FILE *edges);
void     graph_print(graph_t *graph);
void     graph_visit(graph_t *graph,int id);


#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBGRAPH_H_ **/

