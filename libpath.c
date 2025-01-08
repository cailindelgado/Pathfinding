#include "libpath.h"
#include "libstack.h"
#include "libpqueue.h"
#include "libbits.h"

#include <math.h>

float
distance_euclidian(int x1,int y1,int x2,int y2)
{
    return sqrt(((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)));
}

float
distance_manhattan(int x1,int y1,int x2,int y2)
{
    return fabs((float) (x1 - x2)) + fabs((float) (y1 - y2));
}

float
distance_chessboard(int x1,int y1,int x2,int y2)
{
    return fmax(fabs((float) x1 - x2), fabs((float) y1 - y2));
}

void
path_dfs(graph_t* graph,int start,int stop)
{
    /** TODO: IMPLEMENT ME **/
    return;
}

void
path_bfs(graph_t* graph,int start,int stop)
{
    /** TODO: IMPLEMENT ME **/
    return;
}

void
path_bestfirst(graph_t* graph,int start,int stop,
                float (*dmfptr)(int,int,int,int))
{
    /** TODO: IMPLEMENT ME **/
    return;
}

void
path_astar(graph_t* graph,int start,int stop,
                float (*dmfptr)(int,int,int,int))
{
    /** TODO: IMPLEMENT ME **/
    return;
}


graph_t* read_maze(char* file)
{
    FILE *f = NULL;
    graph_t* graph = graph_new();
    int totallength,i,memusage;

    /* read graph from file */
    if ((f = fopen(file, "r")) == NULL) {
        fprintf (stderr, "ERROR: open file\n");
        exit(EXIT_FAILURE);
    }
    fprintf (stdout, "INPUT GRAPH       = %s\n",file);
    graph_read(graph, f);
    fprintf (stdout, "MAZE SIZE         = %dx%d\n",graph->width,graph->height);
    totallength = 0;
    memusage = 0;
    for(i=0;i<graph->order;i++) {
        if( ((dlist_t*)(graph->graph[i]))->list_len > 0 ) totallength++;
        memusage += ((dlist_t*)(graph->graph[i]))->size;
    }
    fprintf (stdout, "MAZE LENGTH       = %d\n",totallength);
    fprintf (stdout, "MEMORY USAGE      = %d bytes\n",memusage);
    fclose(f);
    return graph;
}

dlist_t* read_routes(char* file)
{
    FILE* f = NULL;
    dlist_t* routes = make_empty_list();
    route_t* r;
    int n,start,stop;
    if ((f = fopen(file, "r")) == NULL) {
        fprintf (stderr, "ERROR: open file\n");
        exit(EXIT_FAILURE);
    }
    fprintf (stdout, "INPUT ROUTES      = %s\n",file);
    if( fscanf(f,"%d\n",&n) != 1) {
        fprintf (stderr, "ERROR: unknown routes file format.\n");
        exit(EXIT_FAILURE);
    }
    while(n) {
        if( fscanf(f,"%d - %d",&start,&stop) != 2 ) {
            fprintf (stderr, "ERROR: unknown routes file format.\n");
            exit(EXIT_FAILURE);
        }
        r = safe_malloc(sizeof(route_t));
        r->start = start;
        r->stop = stop;
        insert_in_order(routes,(void*)r,sizeof(route_t),cmp_route);
        n--;
    }
    fprintf (stdout, "ROUTES            = %d\n",routes->list_len);
    fclose(f);
    return routes;
}

