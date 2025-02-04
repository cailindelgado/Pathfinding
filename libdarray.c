#include "libdarray.h"


darray *create_darray(size_t initialCapacity) {

    if (initialCapacity <= 0) {
        initialCapacity = 16;
    }

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

    if (da == NULL) {
        return;
    }

    free(da -> array);
    free(da);

    return;
}

/* resize the array when full */
void resize(darray *da) {
    // create a new array
    // double the capacity

    size_t new_capacity;
    new_capacity = da->capacity * 2;

    void *temp = realloc(da->array, new_capacity * sizeof(int));

    if (temp == NULL) {
        return;
    }

    da->array = temp;
    da->capacity = new_capacity;
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
    if (da->size == da->capacity) {
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

    for (size_t i = 0; i < da->size; i++) {
        if (((int *) da -> array)[i] == elem) {
            idx = i;
            found = 1;
            break;
        }
    }

    if (found) {
        for (size_t i = idx; i < da->size; i++) {
            ((int *) da->array)[i] = ((int *) da->array)[i + 1]; // put i + 1 at i

        }
    }

    da->size -= 1;
}

/* remove the element from the given index returns NULL if not found FREE OUT VARIABLE*/
void *remove_at(darray *da, size_t idx) {

    if (idx >= da->size) { // size_t cannot be negative
        return NULL;
    }

    /* This wil not work, as out is a local variable and its mem will be removed when outside of func */
    int out;
    out = ((int *) da->array)[idx];

    size_t i;
    for (i = idx; i < da->size; i++) {
        ((int *) da->array)[i] = ((int *) da->array)[i + 1];
    }

    int *removed = (int *) malloc(sizeof(int));

    if (removed == NULL) {
        return NULL;
    }

    da->size -= 1;
    *removed = out;

    return removed;
}
