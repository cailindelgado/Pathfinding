#include "libdarray.h"

#include <stdio.h>


void test_create_array(void) {
    darray *da = create_array(10);
    if (da == NULL) {
        printf("create_array failed\n");
        return;
    }
    printf("create_array passed\n");
    free(da->array);
    // free(da);
    // destroy_darray(darray);
}


int main()
{
    printf("begin tests\n");

    test_create_array();

    return (EXIT_SUCCESS);
}
