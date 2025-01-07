
#include "libstack.h"
#include "libutil.h"

istack_t*
stack_new(int size)
{
  istack_t* pst = safe_malloc(sizeof(istack_t));
	pst->size = size;
	pst->top = 0;
	pst->items = safe_malloc(sizeof(int)*size);
    return pst;
}

void
stack_push(istack_t * pst, int item)
{
	assert(pst != NULL);
	assert(pst->items != NULL);

	if (pst->size == pst->top) {
		pst->size = pst->size * 2;
		pst->items = safe_realloc(pst->items,sizeof(int)*pst->size);
	}
	pst->items[pst->top] = item;
	pst->top = pst->top + 1;
}

int
stack_pop(istack_t * pst)
{
	int             val;

	assert(pst != NULL);
	assert(pst->top > 0);
	assert(pst->items != NULL);

	val = pst->items[pst->top - 1];
	pst->top = pst->top - 1;

	return val;
}

int
stack_is_empty(istack_t * pst)
{
	assert(pst != NULL);
	if (pst->top == 0) return TRUE;
	return FALSE;
}

void
stack_free(istack_t* pst)
{
	assert(pst != NULL);
	if(pst->items) free(pst->items);
    free(pst);
}

