#include "assert.h"
#include "libdarray.h"

static void test_create();
static void test_delete_array();
static void test_resize();
static void test_is_full();
static void test_is_empty();
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
    delete_darray(da);

    printf("SUCCESSFUL\n"); 
}

static void test_resize() {
    printf("Testing resizing array...... ");

    darray *da = create_darray(2);

    delete_darray(da);

}

static void test_is_full() {
    printf("Testing if the array is full...... ");

}

static void test_is_empty() {
    printf("Testing if the array is empty...... ");

}

static void test_append() {
    printf("Testing appending to the array...... ");

}

static void test_remove_element() {
    printf("Testing removing an element from the array...... ");

}

static void test_remove_at() {
    printf("Testing removing an element from specified index...... ");

}


int main() {
    /* tests */
    printf("Running tests for darray\n");
    test_create();
    test_delete_array();
    test_resize();
    test_is_full();
    test_is_empty();
    test_append();
    test_remove_element();
    test_remove_at();
}

