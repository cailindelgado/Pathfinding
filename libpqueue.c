#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "libpqueue.h"
#include "libutil.h"
#include "liblist.h"

pqueue_t*
pqueue_new()
{
    /** TODO: IMPLEMENT ME **/
    pqueue_t *pqueue;
    return pqueue;
}

void
pqueue_free(pqueue_t * pque,void (*delfptr)(void*))
{
    /** TODO: IMPLEMENT ME **/
    return;
}

void
pqueue_enqueue(pqueue_t * pque,void* item,int size)
{
    /** TODO: IMPLEMENT ME **/
    return;
}

void
pqueue_enqueue_with_priority(pqueue_t * pque,void* item,int size,
                             int (*cmpfptr)(const void*,const void*))
{
    /** TODO: IMPLEMENT ME **/
    return;
}

void *
pqueue_dequeue(pqueue_t * pque)
{
    /** TODO: IMPLEMENT ME **/
    return (NULL);
}

int
pqueue_isempty(pqueue_t * pque)
{
    /** TODO: IMPLEMENT ME **/
    return (TRUE);
}

void *
pqueue_find(pqueue_t* pq,void* item,int (*cmpfptr)(const void*,const void*))
{
    /** TODO: IMPLEMENT ME **/
    return (NULL);
}

void *
pqueue_remove(pqueue_t* pq,void* item,int (*cmpfptr)(const void*,const void*))
{
    /** TODO: IMPLEMENT ME **/
    return (NULL);
}

