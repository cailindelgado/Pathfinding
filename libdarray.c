#include "libdarray.h"


darray *create_darray(size_t initialCapacity) {

    darray *da = (darray *) malloc(sizeof(darray));
    if (da == NULL)
        return NULL;

    void **array = malloc(initialCapacity * sizeof(void*));
    
    if (array == NULL) {
        free(da);
        return NULL;
    }

    da -> array = array;
    da -> size = 0;
    da -> capacity = initialCapacity;

    return da;
}


void delete_darray(darray *da) {
    
    if (*da->array == NULL) {
        return;
    }

    free(*da -> array);
    free(da);

    return;
}

