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
    
    free(da -> array);
    free(da);

    return;
}

/* TODO: implement or I just have a fancy list */
/* resize the array when full */
void resize(darray *da) {
    // create a new array
    // double the capacity
    //
}

/* check if the array is full */
int is_full(darray *da) {
    if (da->capacity == da->size) {
        return 1;
    } 
    return 0;
}

/* check if the array is empty */
int is_empty(darray *da) {
    if (da->size == 0) {
        return 1;
    }
    return 0;
}

/* add an element to the back of the array */
void append(darray *da, int elem) {
    if (is_full(da)) {
        resize(da);
    }

    ((int *) da -> array)[da->size] = elem; // NOTE: error because array is of type void so needs to be given a type first
    da -> size += 1;
    return;
}

/* remove the first occurance of an element from the array */
void remove_element(darray *da, int elem) {
    int found;
    size_t idx;
    found = 0;

    for (size_t i = da->size; i < da->capacity; i++) {
        if (da->array[i] == elem) {
            idx = i;
            found = 1;
            break;
        }
    }

    int b1;
    int b2;

    if (found) {
        for (size_t i = idx + 1; i < da->capacity; i++) {
            b1 = da->array[i + 1];
            b2 = da->array[i];

            /* swap the two variables */
            b1 = b1 ^ b2;
            b2 = b1 ^ b2;
            b1 = b1 ^ b2;
        }
    }
}

/* remove the element from the given index returns NULL if not found */
void *remove_at(darray *da, size_t idx);

