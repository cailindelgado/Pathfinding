#include "libutil.h"
#include "liblist.h"
#include "libbits.h"
#include "libgraph.h"
#include "libpath.h"

/*
 *  draw nothing to do timings
 */
void
draw_pos(int x,int y)
{
    return;
}

int
main (int argc, char **argv)
{
    graph_t* graph = NULL;
    dlist_t* routes = NULL;
    route_t* r;
    FILE *gfile = NULL, *rfile = NULL;
    float (*dmetricfptr)(int,int,int,int) = NULL;
    int i,n,start,stop;

    /* parse command line parameters */
    clo_t *iopts = parse_args (argc, argv);
    if(iopts->algorithm == BESTFIRST || iopts->algorithm == ASTAR) {
    switch(iopts->metric) {
            case EUCLIDIAN:
                dmetricfptr = distance_euclidian;
                break;
            case MANHATTAN:
                dmetricfptr = distance_manhattan;
                break;
            case CHESSBOARD:
                dmetricfptr = distance_chessboard;
                break;
            default:
                endwin();
                fprintf(stderr,"UNKNOWN DISTANCE METRIC\n");
                exit (EXIT_FAILURE);
        }
    }

    /* read maze */
    graph = read_maze(iopts->edge_file);

    /* read routes */
    routes = read_routes(iopts->routes_file);


    /* timing */
    unsigned long tstart = gettime();

    /* path finding */
    dlnode_t* cur = routes->head;
    uint visited = 0;
    while(cur!=NULL) {
        r = (route_t*)cur->data;
        switch(iopts->algorithm) {
            case DFS:
                path_dfs(graph,r->start,r->stop);
                break;
            case BFS:
                path_bfs(graph,r->start,r->stop);
                break;
            case BESTFIRST:
                path_bestfirst(graph,r->start,r->stop,dmetricfptr);
                break;
            case ASTAR:
                path_astar(graph,r->start,r->stop,dmetricfptr);
                break;
            case UNKNOWN:
            default:
                fprintf(stderr,"UNKNOWN PATHFINDING ALGORITHM\n");
                exit (EXIT_FAILURE);
        }

        /* reset visited */
        for(i=0;i<graph->order;i++) {
            if(is_bit_set(graph->visited,i)==TRUE) visited++;
            unset_bit(graph->visited,i);
        }

        cur = cur->next;
    }

    unsigned long tstop = gettime();

    fprintf (stdout, "VISITED NODES     = %d\n",visited);
    fprintf (stdout, "TIME              = %.3f sec\n",(double)(tstop-tstart) / 1e6);

    /* cleanup */
    delete_list(routes,free);
    graph_delete(graph);
    delete_clo (iopts);

    return (EXIT_SUCCESS);
}

