
#include "libutil.h"
#include "liblist.h"
#include "libbits.h"
#include "libgraph.h"
#include "libpath.h"

#include <curses.h>

/* global maze window */
WINDOW* maze;
WINDOW* gw;
int rtimeout;
int draw;

#define DRAW_TIMEOUT 5000

typedef struct
{
        char *edge_file;
        char *routes_file;
        int width;
        int height;
        int nroutes;
        int verbose;
        int draw;
} clp_t;

clp_t *init_clog(void);
void delete_clog(clp_t *clo);
clp_t * parse_argsg(int argc, char **argv);

static void
print_usageg(const char *program)
{
    fprintf (stderr, "USAGE: %s [options]\n", program);
    fprintf (stderr, "    -w maze width\n");
    fprintf (stderr, "    -h maze height \n");
    fprintf (stderr, "    -n number of routes to generate\n");
    fprintf (stderr, "    -d draw generation of maze <width/height == "
             "screen size>\n");
    fprintf (stderr, "    -e graph edge output\n");
    fprintf (stderr, "    -r graph route output\n");
    fprintf (stderr, "\n");
    fprintf (stderr, "EXAMPLE: %s -w 640 -h 480 -n 20 -e test.graph -r "
                     "test.routes\n",program);
    fprintf (stderr, "\n");
    return;
}

/** Initialize resources for the command line options **/
clp_t *
init_clog(void)
{
    clp_t *rv = NULL;
    rv = (clp_t *) safe_malloc (sizeof (clp_t));
    rv->edge_file = NULL;
    rv->routes_file = NULL;
    rv->width = 0;
    rv->height = 0;
    rv->nroutes = 0;
    rv->draw = FALSE;
    draw = FALSE;
    return (rv);
}

/** Free all resources allocated for the command line options **/
void
delete_clog(clp_t *noret)
{
    if (!noret) {
        fprintf (stderr,"WARNING: command line options does not exist!\n");
        return;
    }
    if (noret->edge_file) free (noret->edge_file);
    if (noret->routes_file) free (noret->routes_file);
    free (noret);
}

clp_t *
parse_argsg(int argc, char **argv)
{
    int opt = GETOPT_FINISHED;
    clp_t * iopts = init_clog();

    if (argc <= 1) {
        print_usageg(argv[0]);
        delete_clog(iopts);
        exit (EXIT_FAILURE);
    }

    while ((opt = getopt (argc, argv, "de:r:w:n:h:")) != GETOPT_FINISHED) {
        switch (opt) {
            case 'd':
                iopts->draw = TRUE;
                draw = TRUE;
                break;
            case 'e':
                iopts->edge_file = safe_strdup (optarg);
                break;
            case 'r':
                iopts->routes_file = safe_strdup (optarg);
                break;
            case 'w':
                iopts->width = atoi(optarg);
                break;
            case 'n':
                iopts->nroutes = atoi(optarg);
                break;
            case 'h':
                iopts->height = atoi(optarg);
                break;
            default:
                print_usageg(argv[0]);
                delete_clog(iopts);
                exit (EXIT_FAILURE);
        }
    }
    if (!iopts->edge_file || !iopts->routes_file || iopts->nroutes <= 0) {
        print_usageg(argv[0]);
        delete_clog(iopts);
        exit (EXIT_FAILURE);
    }

    if(iopts->draw == FALSE && (iopts->width ==0 || iopts->height==0)) {
        fprintf(stderr,"ERROR: NO WIDTH/HEIGHT SPECIFIED IN NON-DRAW MODE\n");
        print_usageg(argv[0]);
        delete_clog(iopts);
        exit (EXIT_FAILURE);
    }

    return (iopts);
}


/*
 * initialize the ncurses library
 */
void
init_curses()
{
    initscr();
    cbreak();
    noecho();
    start_color();
    maze = NULL;
    gw = NULL;
    atexit( (void(*)())endwin );
}

/*
 * draw game window box
 */
void
init_screen()
{
    int width,height,i,j;

    /* del if they already exist*/
    if(maze) delwin(maze);
    if(gw) delwin(gw);

    /* create game window + border */
    getmaxyx(stdscr,height,width);
    gw = newwin(0,0,1,0);
    box(gw,0,0);
    mvwprintw(stdscr,0,0,"Press 'q' to quit.");
    mvwprintw(stdscr,0,width-strlen("Speed: press + or -")-2
                        ,"Speed: press + or -");

    wrefresh(stdscr);
    wrefresh(gw);

    /* get the number of rows and columns */
    getmaxyx(gw,height,width);
    maze = newwin(height-2,width-2,2,1);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    wattron(maze,COLOR_PAIR(1));
    for(i=0;i<height;i++) {
        for(j=0;j<width;j++)
            mvwaddch(maze,i,j,' '|A_REVERSE);
    }
    wrefresh(maze);

    /* wait at least 25 microseconds between drawings */
    rtimeout = DRAW_TIMEOUT;
    wtimeout(maze,0);
}


/*
 *  draw a section of the maze
 */
void
draw_pos(int x,int y)
{
    if(draw) {
        int ch;

        mvwaddch(maze,y,x,' ');
        wrefresh(maze);

        if(rtimeout>0) {
            usleep(rtimeout);
        }

        if( ch = wgetch(maze) ) {
            if( ch == 'q' ) {
                exit(EXIT_SUCCESS);
            }
            if( ch == '+' ) {
                rtimeout-= 5;
                if(rtimeout<0) rtimeout = 0;
            }
            if( ch == '-' ) {
                rtimeout+= 5;
            }
        }
    }
}

void
draw_route(graph_t* g,int start,int stop)
{
    if(draw) {
        int startx,starty,stopx,stopy;
        startx = start % g->width;
        starty = start / g->width;

        stopx = stop % g->width;
        stopy = stop / g->width;


        wattron(maze,COLOR_PAIR(2));
        mvwaddch(maze,stopy,stopx,'$');
        wattron(maze,COLOR_PAIR(3));
        mvwaddch(maze,starty,startx,'@');
    }
}


void
graph_create(graph_t* graph,int w,int h)
{
    graph->order = w*h;
    graph->width = w;
    graph->height = h;
    graph->graph = NULL;
    graph->visited = new_bitvector(graph->order);
}

int*
graph_getadjacent(graph_t* g,int id)
{
    static int adj[4];
    int w;
    w = g->width;
    
    if(id%w!=0) adj[0] = id-1;
    else adj[0] = -1;

    if(id%w!=w-1) adj[1] = id+1;
    else adj[1] = -1;

    if(id>w) adj[2] = id-w;
    else adj[2] = -1;

    if(id<g->order-w) adj[3] = id+w;
    else adj[3] = -1;

    return (int*)&adj;
}

graph_addadjacent(graph_t* graph,int id,dlist_t* walllist)
{
    int *ndat;
    int i;
    int* adj = graph_getadjacent(graph,id);
    for(i=0;i<4;i++) {
        if( adj[i] == -1 ) continue;

        if( is_bit_set(graph->visited,adj[i]) == FALSE ) {
            ndat = safe_malloc(sizeof(int));
            *ndat = adj[i];
            insert_at_front(walllist,(void*)ndat,sizeof(int));
        }
    }
}

int
graph_checkwall(graph_t* g,int wall)
{
    int origin,i;
    /* if wall is already in the maze we don't do anything */
    if( is_bit_set(g->visited,wall)) return -1;

    /* find the wall we came from */
    int* adj = graph_getadjacent(g,wall);
    for(i=0;i<4;i++) {
        if(adj[i] != -1 && is_bit_set(g->visited,adj[i]) ) {
            origin = adj[i];
            break;
        }
    }

    if(origin == wall-1) { /* we come from the left */
        if( (wall+1) % g->width != 0 ) { /* can we go to the right */
            if( is_bit_set(g->visited,wall+1) == FALSE) { /* is right already in maze */
                return wall+1;
            }
        }
    }

    if(origin == wall+1) { /* we come from the right */
        if( wall>0 && (wall-1) % g->width != g->width-1 ) { /* can we go to the left */
            if( is_bit_set(g->visited,wall-1) == FALSE) { /* is left already in maze */
                return wall-1;
            }
        }
    }

    if(origin == wall-g->width) { /* we come from the top */
        if( (wall+g->width) < g->order ) { /* can we go to down */
            if( is_bit_set(g->visited,wall+g->width) == FALSE) { /* is bottom already in maze */
                return wall+g->width;
            }
        }
    }

    if(origin == wall+g->width) { /* we come form the bottom */
        if( (wall-g->width) >= 0 ) { /* can we go to the top */
            if( is_bit_set(g->visited,wall-g->width) == FALSE) { /* is top already in maze */
                return wall-g->width;
            }
        }
    }

    return -1;
}

void
graph_genmaze(graph_t* graph)
{
    int i,v;
    int* tmp;
    dlist_t* wlist;
    wlist = make_empty_list();

    /* pick random vertex as starting point*/
    i = rand() % graph->order;
    graph_visit(graph,i);

    /* add adjacent vertices to list of walls */
    graph_addadjacent(graph,i,wlist);

    /* while candidate walls are available */
    while( wlist->list_len > 0 ) {
        /* choose random candidate */
        tmp = (int*) remove_random(wlist);
        /* check if opposite side of the wall is not part of the maze yet*/
        v = graph_checkwall(graph,*tmp);

        /* mark wall as passage and add opposite side to maze */
        if(v!=-1) {
            graph_visit(graph,*tmp);
            graph_visit(graph,v);
            /* add walls around opposite side to maze */
            graph_addadjacent(graph,v,wlist);
        }

        free(tmp);
    }

    delete_list(wlist,free);
}



void
generate_routes(graph_t* g,dlist_t* routes,int n)
{
    int start,stop,i,j;
    for(i=0;i<n;i++) {
        /* pick a random starting point */
        start = rand() % g->order;
        j = 0;
        while(start>0) {
            j = (j+1) % g->order;
            if(is_bit_set(g->visited,j)) {
                start--;
            }
        }
        start = j;

        stop = rand() % g->order;
        j = 0;
        while(stop>0) {
            j = (j+1) % g->order;
            if(is_bit_set(g->visited,j)) {
                stop--;
            }
        }
        stop = j;

        route_t* r = safe_malloc(sizeof(route_t));
        r->start = start;
        r->stop = stop;


        draw_route(g,start,stop);

        insert_in_order(routes,(void*)r,sizeof(route_t),cmp_route);
    }
}


int
main (int argc, char **argv)
{
    graph_t* graph = NULL;
    dlist_t* routes = NULL;
    FILE *gfile = NULL,*rfile = NULL;
    int i,j,stdscrh,strscrw;
    dlnode_t* cnode;

    /* init random num generator */
    struct timeval tp;
    gettimeofday(&tp,NULL);
    srand((unsigned int)tp.tv_sec%tp.tv_usec);

    /* parse command line parameters */
    clp_t *iopts = parse_argsg(argc, argv);

    if(iopts->draw) {
        /* init ncurses display */
        init_curses();
        /* create display windows */
        init_screen();

        /* we are getting the graph dimensions from the terminal size */
        getmaxyx(stdscr,stdscrh,strscrw);
        getmaxyx(maze,iopts->height,iopts->width);
    } else {
        fprintf(stdout,"Creating graph (%d,%d)... ",iopts->width,iopts->height);
        fflush(stdout);
    }

    /* init maze */
    graph = graph_new();

    /* generate maze */
    graph_create(graph,iopts->width,iopts->height);


    if(iopts->draw) {
        mvwprintw(stdscr,0,strscrw/2-(strlen("Generating maze...")/2)
                ,"Generating maze...");
        wrefresh(stdscr);
    } else {
        fprintf(stdout,"done.\n");
        fprintf(stdout,"Generating maze (%d,%d)... ",iopts->width,iopts->height);
        fflush(stdout);
    }
    graph_genmaze(graph);

    if(iopts->draw) {
        mvwprintw(stdscr,0,strscrw/2-(strlen("Creating routes...")/2)
                ,"Creating routes...");
        wrefresh(stdscr);
    } else {
        fprintf(stdout,"done.\n");
        fprintf(stdout,"Creating %d routes... ",iopts->nroutes);
        fflush(stdout);
    }

    /* create routes */
    routes = make_empty_list();
    generate_routes(graph,routes,iopts->nroutes);

    /* write stuff to file */
    if(iopts->draw) {
        mvwprintw(stdscr,0,strscrw/2-(strlen("Writing to file...")/2)
                ,"Writing to file...");
        wrefresh(stdscr);
    } else {
        fprintf(stdout,"done.\n");
        fprintf(stdout,"Writing to file... ");
        fflush(stdout);
    }

    /* write graph */
    if ((gfile = fopen(iopts->edge_file, "w")) == NULL) {
        fprintf (stderr, "ERROR: open file\n");
        return (EXIT_FAILURE);
    } else {
        fprintf(gfile,"%d %d %d\n",graph->order,graph->width,graph->height);
        /* write adjacency lists */
        for(i=0;i<graph->order;i++) {
            if(is_bit_set(graph->visited,i)) {
                int* adj = graph_getadjacent(graph,i);
                for(j=0;j<4;j++) {
                    if( adj[j] == -1 ) continue;

                    if( is_bit_set(graph->visited,adj[j]) )
                        fprintf(gfile,"%d - %d\n",i,adj[j]);
                }
            }
        }
        fclose(gfile);
    }

    /* write routes */
    if ((rfile = fopen(iopts->routes_file, "w")) == NULL) {
        fprintf (stderr, "ERROR: open file\n");
        return (EXIT_FAILURE);
    } else {
        fprintf(rfile,"%d\n",routes->list_len);
        /* write adjacency lists */
        cnode = routes->head;
        while(cnode!=NULL) {
            route_t* r = (route_t*)cnode->data;
            fprintf(rfile,"%d - %d\n",r->start,r->stop);
            cnode = cnode->next;
        }
        fclose(rfile);
    }


    if(iopts->draw) {
        mvwprintw(stdscr,0,strscrw/2-(strlen("all done. press 'q' to quit.")/2)
                ,"all done. press 'q' to quit.");
        wrefresh(stdscr);
        wtimeout(maze,-1); /* blocking */
        while( wgetch(maze) != 'q') {
            wrefresh(maze);
        };
    } else {
        fprintf(stdout,"done.\n");
    }

    /* cleanup */
    delete_list(routes,free);
    delete_clog(iopts);
    /* manually delete graph */
    delete_bitvector(graph->visited);
    free(graph);

    return (EXIT_SUCCESS);
}

