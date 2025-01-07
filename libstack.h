/* stack.h */

#ifndef LIBSTACK_H
#define LIBSTACK_H

#define INITIAL_STACK_SIZE 16

#include "libutil.h"

typedef struct {
	int    size;
	int    top;
	int    *items;
} istack_t;

istack_t* stack_new(int size);
void stack_push(istack_t * stack, int item);
int  stack_pop(istack_t * stack);
int  stack_is_empty(istack_t * stack);
void stack_free(istack_t*);

#endif
