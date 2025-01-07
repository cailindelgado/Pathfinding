#include "libutil.h"
#include "liblist.h"
#include "libbits.h"
#include "libgraph.h"
#include "libpath.h"

#include <curses.h>

/* global ncurses window vars*/
WINDOW* maze;
WINDOW* gw;
int rtimeout;
int drawpath;

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
	/* register endwin() to be called on exit */
    atexit( (void(*)())endwin );
}

/*
 * draw game window box
 */
void
init_screen(graph_t* g)
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

    /* if the maze is smaller we center it */
    if(g->height > height-2 || g->width > width-2) {
        mvwprintw(stdscr,0,width/2-
              (strlen("Maze larger than screen. Only displayed partially.")/2)
                ,"Maze larger than screen. Only displayed partially.");
        wrefresh(stdscr);
    }

    maze = newwin(g->height,g->width,2,1);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_CYAN);
    wattron(maze,COLOR_PAIR(1));
    for(i=0;i<height;i++) {
            for(j=0;j<width;j++)
                    mvwaddch(maze,i,j,' '|A_REVERSE);
    }
    wrefresh(maze);
    rtimeout = 0;
    wtimeout(maze,0);
    drawpath = 0;
}


/*
 *  draw a section of the maze
 */
void
draw_pos(int x,int y)
{
    int ch;
    mvwaddch(maze,y,x,' ');
    wrefresh(maze);
    if(rtimeout) {
        usleep(rtimeout);
    }
    if( ch = wgetch(maze) ) {
        if( ch == 'q' ) {
            exit(EXIT_SUCCESS);
        }
        if( ch == '+' ) {
            rtimeout-= 5; /* make it 5ms faster*/
            if(rtimeout<5) rtimeout = 0;
        }
        if( ch == '-' ) {
            rtimeout+= 5; /* make it 5ms slower*/
        }
    }
}

void
draw_route(graph_t* g,int start,int stop)
{
    int startx,starty,stopx,stopy;
    startx = start % g->width;
    starty = start / g->width;

    stopx = stop % g->width;
    stopy = stop / g->width;

    wattron(maze,COLOR_PAIR(2));
    mvwaddch(maze,stopy,stopx,'$');
    wattron(maze,COLOR_PAIR(3));
    mvwaddch(maze,starty,startx,'@');
    wrefresh(maze);
}

void
draw_maze(graph_t* g)
{
    int i,j,x,y;
    int width,height;
    getmaxyx(maze,height,width);
    wattron(maze,COLOR_PAIR(1));
    for(i=0;i<height;i++) {
            for(j=0;j<=width;j++)
                    mvwaddch(maze,i,j,' '|A_REVERSE);
    }
    wrefresh(maze);

    for(i=0;i<g->order;i++) {
        dlist_t* al = (dlist_t*)g->graph[i];
        if(al && al->list_len > 0) {
            x = i % g->width;
            y = i / g->width;
            draw_pos(x,y);
        }
    }
}

int
main (int argc, char **argv)
{
    graph_t* graph = NULL;
    dlist_t* routes = NULL;
    route_t* r;
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
                fprintf(stderr,"UNKNOWN DISTANCE METRIC\n");
                exit (EXIT_FAILURE);
        }
    }

    /* read maze */
    graph = read_maze(iopts->edge_file);

    /* read routes */
    routes = read_routes(iopts->routes_file);

    /* draw graph */
    init_curses();
    init_screen(graph);
    draw_maze(graph);

    /* path finding */
    dlnode_t* cur = routes->head;
    while(cur!=NULL) {
        r = (route_t*)cur->data;

        /* increase draw timeout to make things "visible" */
        rtimeout = 7500;
        draw_route(graph,r->start,r->stop);

        /* next we are drawing our path! set the color */
        wattron(maze,COLOR_PAIR(4));

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

        /* sleep for a while so we can look a the result */
        sleep(3);

        /* clear maze */
        rtimeout = 0;
        draw_maze(graph);

        /* reset visited */
        for(i=0;i<graph->order;i++) unset_bit(graph->visited,i);

        cur = cur->next;
    }

    /* wait for quit */
    int strscrw,strscrh;
    getmaxyx(stdscr,strscrh,strscrh);
    mvwprintw(stdscr,0,strscrw/2-(strlen("all done. press 'q' to quit.")/2)
            ,"all done. press 'q' to quit.");
    wrefresh(stdscr);
    wtimeout(maze,-1); /* blocking */
    while( wgetch(maze) != 'q') {
        wrefresh(maze);
    };

    /* cleanup */
    delete_list(routes,free);
    graph_delete(graph);
    delete_clo (iopts);

    return (EXIT_SUCCESS);
}

