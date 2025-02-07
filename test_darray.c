#include "assert.h"
#include "libdarray.h"
#include "stdlib.h"
#include "time.h"

static void test_create();
static void test_delete_array();
static void test_resize();
static void test_full_empty();
static void test_append();
static void test_remove_element();
static void test_remove_at();
static void test_insert_resize_remove_middle();

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

static void test_insert_resize_remove_middle() {
    printf("Testing inserting tons of numbers and removing some... ");

    time_t start, end;
    size_t BIG_NUM;
    BIG_NUM = 10000000;

    start = time(NULL);
    darray *da = create_darray(23);

    /* NOTE: This line is problem child, there is a stack overflow because BIG_NUM exceeds stack size limit
    int against[BIG_NUM]; // NOTE: doesnt have the same functionality as implemented da
    */

    // allocating memory on the heap in order to not exceed stack size limit
    int *against = malloc(BIG_NUM * sizeof(int));
    if (against == NULL) {
        printf("Mem allocation for against failed\n");
        return;
    }

    int x;
    size_t i;
    for (i = 0; i < BIG_NUM; i++) {
        x = rand() % 100 + 1;

        append(da, x);
        against[i] = x;
    }

    assert(da->size == BIG_NUM);
    assert(((int *)da->array)[da->size - 1] == against[da->size - 1]);

    for (i = 0; i < BIG_NUM; i++) {
        assert(((int *) da->array)[i] == against[i]);
    }

    end = time(NULL);

    printf("SUCCESSFUL: in %lds\n", end - start);
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
    test_insert_resize_remove_middle();

    printf("TESTS COMPLETE FOR DYNAMIC ARRAY\n");
}
