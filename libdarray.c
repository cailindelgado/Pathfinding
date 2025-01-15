#include "libdarray.h"

darray *create_array(size_t initialCapacity) {
    darray *da = (darray *)malloc(sizeof(darray));
    if (da == NULL) {
        return (NULL);
    }

    da->array = (int *)malloc(sizeof(int) * initialCapacity);
    if (da->array == NULL) {
        return (NULL);
    }

    da->size = 0;
    da->capacity = initialCapacity;
    return da;
}
