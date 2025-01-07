/*** Miscelaneous Utility Functions ***/
#ifndef _LIBUTIL_H_
#define _LIBUTIL_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <getopt.h>

#define GETOPT_FINISHED -1
#define REALLOC_INCREMENT 256
#define MAX_WORD_LEN 64

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* Create a new block of memory and do vital error checking.
 * The return value should be free'd by the calling function.
 */
void *safe_malloc (size_t size);

/* Resize a pre-existing block of memory and do vital error checking.
 * The return value should be free'd by the calling function.
 */
void *safe_realloc (void *old_mem, size_t new_size);

/* Take a current string and create a new copy. The
 * return value should be free'd by the calling function.
 */
char *safe_strdup (const char *str);

/* Compare two integers. Return 0 if equal, negative if a < b, positive
 * if a > b.
 */
int cmp_int (const void *a, const void *b);


/* timing */
unsigned long gettime();

/* command line parsing */
typedef enum
{
    EUCLIDIAN,
    MANHATTAN,
    CHESSBOARD,
    ERROR,
} dmetric_t;

typedef enum
{
    DFS,
    BFS,
    BESTFIRST,
    ASTAR,
    UNKNOWN
} palg_t;


typedef struct
{
    char *edge_file;
    char *routes_file;
    dmetric_t metric;
    palg_t algorithm;
} clo_t;

clo_t *init_clo (void);
void delete_clo (clo_t *clo);
clo_t * parse_args (int argc, char **argv);
void print_usage (const char *program);

#ifdef __cplusplus
}
#endif /** cplusplus **/

#endif /** _LIBUTIL_H_ **/

