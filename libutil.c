#include "libutil.h"

/*  safe_malloc ()
 *
 *  Call calloc () and abort if the specified amount of
 *  memory cannot be allocated.
 */
void*
safe_malloc (size_t size)
{
    void *mem_block = NULL;
    if ((mem_block = calloc (1, size)) == NULL) {
        fprintf (stderr, "ERROR: safe_malloc(%zu) cannot allocate memory.",
             size);
        exit (EXIT_FAILURE);
    }
    return (mem_block);
}

/*  safe_realloc ()
 *
 *  Call realloc () and abort if the specified amount of
 *  memory cannot be allocated.
 */
void*
safe_realloc (void *old_mem, size_t new_size)
{
    if ((old_mem = realloc (old_mem, new_size)) == NULL) {
        fprintf (stderr, "ERROR: safe_realloc() cannot allocate"
                "%zu blocks of memory.\n", new_size);
        exit (EXIT_FAILURE);
    }
    return (old_mem);
}

/*
 * safe_strdup ()
 *
 * Safe version of strdup avoid buffer overflow, etc.
 *
 */
char*
safe_strdup (const char *str)
{
    char *copy = NULL;
    if (str == NULL) {
        fprintf (stderr, "ERROR safe_strdup(): str == NULL");
        exit (EXIT_FAILURE);
    }
    copy = safe_malloc ((strlen (str) + 1) * sizeof (char));
    (void) strcpy (copy, str);
    return (copy);
}

/* Compare two integers. Return 0 if equal, negative if a < b, positive
 * if a > b.
 */
int
cmp_int (const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

/* timing */
unsigned long
gettime()
{
    struct timeval tp ;
    gettimeofday(&tp,NULL);
    return (tp.tv_sec*1E6) + tp.tv_usec;
}

/* command line parsing */
void
print_usage (const char *program)
{
    fprintf (stderr, "USAGE: %s [options]\n", program);
    fprintf (stderr, "    -e graph input file\n");
    fprintf (stderr, "    -r routes input file\n");
    fprintf (stderr, "    -a algorithm (dfs/bfs/bestfirst/astar)\n");
    fprintf (stderr, "    -d distance metric (euclidian/manhattan/chessboard)\n");
    fprintf (stderr, "    -h display usage information\n");
    fprintf (stderr, "\n");
    fprintf (stderr, "EXAMPLE: %s -e test.graph -r test.routes\n",
                     program);
    fprintf (stderr, "\n");
    return;
}


/** Initialize resources for the command line options **/
clo_t *
init_clo (void)
{
    clo_t *rv = NULL;
    rv = (clo_t *) safe_malloc (sizeof (clo_t));
    rv->edge_file = NULL;
    rv->routes_file = NULL;
    rv->algorithm = UNKNOWN;
    rv->metric = ERROR;
    return (rv);
}

/** Free all resources allocated for the command line options **/
void
delete_clo (clo_t *noret)
{
    if (!noret) {
        fprintf (stderr,"WARNING: command line options does not exist!\n");
        return;
    }
    if (noret->edge_file) free (noret->edge_file);
    if (noret->routes_file) free (noret->routes_file);
    free (noret);
}

clo_t *
parse_args (int argc, char **argv)
{
    int opt = GETOPT_FINISHED;
    clo_t * iopts = init_clo ();

    if (argc <= 1) {
        print_usage (argv[0]);
        delete_clo (iopts);
        exit (EXIT_FAILURE);
    }

    while ((opt = getopt (argc, argv, "e:r:a:d:h")) != GETOPT_FINISHED) {
        switch (opt) {
            case 'e':
                iopts->edge_file = safe_strdup (optarg);
                break;
            case 'r':
                iopts->routes_file = safe_strdup (optarg);
                break;
            case 'd':
                if (strcmp (optarg, "euclidian") == 0) {
                    fprintf (stdout, "DISTANCE METRIC   = euclidian\n");
                    iopts->metric = EUCLIDIAN;
                } else if (strcmp (optarg, "manhattan") == 0) {
                    fprintf (stdout, "DISTANCE METRIC   = manhattan\n");
                    iopts->metric = MANHATTAN;
                } else if (strcmp (optarg, "chessboard") == 0) {
                    fprintf (stdout, "DISTANCE METRIC   = chessboard\n");
                    iopts->metric = CHESSBOARD;
                } else {
                    fprintf (stderr, "ERROR: <%s> unknown distance metric!\n", optarg);
                    delete_clo (iopts);
                    exit (EXIT_FAILURE);
                }
                break;
            case 'a':
                if (strcmp (optarg, "dfs") == 0) {
                    iopts->algorithm = DFS;
                    fprintf (stdout, "ALGORITHM         = dfs\n");
                } else if (strcmp (optarg, "bfs") == 0) {
                    fprintf (stdout, "ALGORITHM         = bfs\n");
                    iopts->algorithm = BFS;
                } else if (strcmp (optarg, "bestfirst") == 0) {
                    fprintf (stdout, "ALGORITHM         = bestfirst\n");
                    iopts->algorithm = BESTFIRST;
                } else if (strcmp (optarg, "astar") == 0) {
                    fprintf (stdout, "ALGORITHM         = astar\n");
                    iopts->algorithm = ASTAR;
                } else {
                    fprintf (stderr, "ERROR: <%s> unknown algorithm!\n", optarg);
                    delete_clo (iopts);
                    exit (EXIT_FAILURE);
                }
                break;
            case 'h':
                print_usage (argv[0]);
                break;
            default:
                print_usage (argv[0]);
                delete_clo (iopts);
                exit (EXIT_FAILURE);
        }
    }
    if (!iopts->edge_file || !iopts->routes_file || iopts->algorithm==UNKNOWN) {
        print_usage (argv[0]);
        delete_clo (iopts);
        exit (EXIT_FAILURE);
    }

    return (iopts);
}


