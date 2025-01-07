/*** Utility Types ***/
#ifndef _LIBPATH_H_
#define _LIBPATH_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/

#include "libutil.h"
#include "libgraph.h"

/* best first search */
typedef struct {
    int id;
    float dist;
} bfitem_t;

/* astar */
typedef struct {
    int id;
    float g;
    float h;
    float f;
} aselem_t;

float distance_euclidian (int x1,int y1,int x2,int y2);
float distance_manhattan (int x1,int y1,int x2,int y2);
float distance_chessboard (int x1,int y1,int x2,int y2);
graph_t* read_maze (char* file);
dlist_t* read_routes (char* file);
void path_dfs (graph_t* graph,int start,int stop);
void path_bfs (graph_t* graph,int start,int stop);
void path_bestfirst (graph_t* graph,int start,int stop,
        float (*dmetricfptr)(int,int,int,int));
void path_astar (graph_t* graph,int start,int stop,
                float (*dmetricfptr)(int,int,int,int));

#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBPATH_H **/

