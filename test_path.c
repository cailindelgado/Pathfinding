#include "libpath.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

# define ERROR 0.0000001
/* Points are (x1, y1), (x2, y2) */

int test_euclid() {

    printf("Begining euclidian distance tests\n");
    /* Points (1, 1) and (2, 2) */
    printf("%f\n", distance_euclidian(1, 1, 2, 2) - sqrt(2));
    /* added a estimated error for rounding error*/
    assert(distance_euclidian(1, 1, 2, 2) < sqrt(2) + ERROR && distance_euclidian(1, 1, 2, 2) > sqrt(2) - ERROR);

    return (EXIT_SUCCESS);
}

int test_manhattan() {

    printf("Begining manhattan distance tests\n");
    /* Points (1, 1) and (2, 2) */
    assert(distance_manhattan(1, 1, 2, 2) == 2);

    return (EXIT_SUCCESS);
}

int test_cheby() {

    printf("Begining cheby distance tests\n");
    /* Points (1, 1) and (2, 2) */
    printf("%f\n", distance_chessboard(1, 1, 2, 2)); /* WHY DOES THIS RETURN 2??????? */
    assert(distance_chessboard(1, 1, 2, 2) == 1);

    return (EXIT_SUCCESS);
}

int main()
{
    printf("begin tests\n");

    test_euclid();
    test_manhattan();
    test_cheby();

    return (EXIT_SUCCESS);
}
