/*** Utility Types ***/

#ifndef _LIBDARRAY_H_
#define _LIBDARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif /** cplusplus **/


# include "libutil.h"

typedef struct {
  void** array;
  size_t size;
  size_t capacity;
} darray;

/* Initialise a new dynamic array */
darray *create_darray(size_t initialCapacity);

/* delete the array and free up all the memory */
void delete_darray(darray *da);

/* resize the array when full */
void resize(darray *da);

/* check if the array is full */
size_t is_full();

/* check if the array is empty */
size_t is_empty();

/* return the current number of elements in the array */
size_t get_size(darray *da);

/* return the current capacity of the array */
size_t get_capacity(darray *da);

/* add an element to the back of the array */
void append(darray *da, int elem);

/* remove the first occurance of an element from the array */
void remove_element(darray *da, int elem);

/* remove the element from the given index returns NULL if not found */
void *remove_at(darray *da, size_t idx);


#ifdef __cplusplus
}
#endif /** cpluplus **/

#endif /** _LIBDARRAY_H_ **/
