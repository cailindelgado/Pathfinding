#include "assert.h"
#include "libdarray.h"

static void test_create();
static void test_delete_array();
static void test_resize();
static void test_full_empty();
static void test_append();
static void test_remove_element();
static void test_remove_at();

static void test_create(void) {
    printf("Testing create_darray..... ");

    darray *da = create_darray(16);

    assert(da != NULL);
    assert(da->size == 0);
    assert(da->capacity == 16);

    delete_darray(da);

    printf("SUCCESSFUL\n"); 
}

static void test_delete_array() {
    printf("Testing deleting the array..... ");

    darray *da = create_darray(2);

    delete_darray(da);

    // Attempting to delete darray
    //delete_darray(da); // why does this cause a segfault???

    printf("SUCCESSFUL\n"); 
}

static void test_resize() {
    printf("Testing resizing array...... ");

    darray *da = create_darray(2);

    assert(da->size == 0);

    append(da, 1);

    assert(da->capacity == 2);
    assert(da->size == 1);

    append(da, 2);

    assert(da->capacity == 2);
    assert(da->size == 2);

    append(da, 3);

    assert(da->capacity == 4);
    assert(da->size == 3);

    assert(((int *) da->array)[0] == 1);
    assert(((int *) da->array)[1] == 2);
    assert(((int *) da->array)[2] == 3);

    delete_darray(da);

    printf("SUCCESSFUL\n"); 
}

static void test_full_empty() {
    printf("Testing if the array is empty...... ");

    darray *da = create_darray(2);


    assert(is_empty(da));

    append(da, 1);
    append(da, 2);

    assert(!is_empty(da));
    assert(da->size == da->capacity);

    delete_darray(da);

    printf("SUCCESSFUL\n"); 
}

static void test_append() {
    printf("Testing appending to the array...... ");

    darray *da = create_darray(16);

    int i;
    for (i = 0; i < 24; i++) {
        append(da, i);
    }

    int j;
    for (j = 0; j < 24; j++) {
        assert(((int *) da->array)[j] == j);
    }

    delete_darray(da);

    printf("SUCCESSFUL\n"); 
}

static void test_remove_element() {
    printf("Testing removing an element from the array...... ");

    darray *da = create_darray(4);

    append(da, 1);
    append(da, 2);
    append(da, 1);

    remove_element(da, 1);

    assert(((int *) da->array)[0] == 2);
    assert(((int *) da->array)[1] == 1);

    delete_darray(da);

    printf("SUCCESSFUL\n");
}

static void test_remove_at() {
    printf("Testing removing an element from specified index...... ");

    darray *da = create_darray(3);

    append(da, 1);
    append(da, 2);
    append(da, 3);

    int* removed = remove_at(da, 3);

    assert(removed == NULL);
    assert(da->size == da->capacity);

    assert(((int *) da->array)[0] == 1);
    assert(((int *) da->array)[1] == 2);
    assert(((int *) da->array)[2] == 3);

    removed = remove_at(da, 1);

    assert(*removed == 2);
    assert(da->size == 2);

    assert(((int *) da->array)[0] == 1);
    assert(((int *) da->array)[1] == 3);

    delete_darray(da);

    printf("SUCCESSFUL\n");
}


int main() {
    /* tests */
    printf("Running tests for darray\n");

    test_create();
    test_delete_array();
    test_resize();
    test_full_empty();
    test_append();
    test_remove_element();
    test_remove_at();

    printf("TESTS COMPLETE FOR DYNAMIC ARRAY\n");
}
