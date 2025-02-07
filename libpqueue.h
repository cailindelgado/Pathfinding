/* queue.h */

#ifndef LIBPQUEUE_H
#define LIBPQUEUE_H

// #include "liblist.h"
#include "libdarray.h"

typedef struct pqueue_t 
{
    // dlist_t* dl;
    darray* da;
} pqueue_t;

pqueue_t* pqueue_new();
int       pqueue_isempty(pqueue_t *);
void      pqueue_enqueue(pqueue_t *, void* item,int size);
void      pqueue_enqueue_with_priority(pqueue_t *,void*,int,
                      int (*cmpfptr)(const void*,const void*));
void      pqueue_free(pqueue_t *,void (*delfptr)(void*));
void*     pqueue_dequeue(pqueue_t *);
void*     pqueue_find(pqueue_t *,void* item,
                      int (*cmpfptr)(const void*,const void*));
void*     pqueue_remove(pqueue_t*,void* item,
                      int (*cmpfptr)(const void*,const void*));

#endif
