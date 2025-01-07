#include "liblist.h"

dlist_t*
make_empty_list (void)
{
	dlist_t *list = NULL;
	list = (dlist_t *) safe_malloc (sizeof (dlist_t));
	assert (list != NULL);
	list->head = list->foot = NULL;
    list->list_len = 0;
    list->size = sizeof (dlist_t);
	return (list);
}

void
delete_list (dlist_t *list, void (*destroy_fptr)(void *))
{
	dlnode_t *curr = NULL, *prev = NULL;
	assert (list != NULL);
    curr = list->head;
	while (curr != NULL) {
		prev = curr;
		curr = curr->next;
        destroy_fptr (prev->data);
		free (prev);
	}
    list->list_len = 0;
	free (list);
}

void
insert_at_front (dlist_t *list, void *value, size_t size)
{
    dlnode_t *new = NULL;
    dlnode_t *tmp = NULL;
    new = (dlnode_t *) safe_malloc (sizeof (dlnode_t));
    assert (list != NULL && new != NULL);
    new->data = value;
    new->next = NULL;
    new->prev = NULL;
    if(list->head == NULL) {
        list->head = new;
        list->foot = new;
    } else {
        tmp = list->head;
        list->head = new;
        new->next = tmp;
        tmp->prev = new;
    }
    list->list_len++;
    list->size += size;
    list->size += sizeof (dlnode_t);
}

void
insert_in_order (dlist_t *list, void *value, size_t size,
                                 int (*cmp_fptr)(const void *, const void *))
{
    dlnode_t *new = NULL;
    dlnode_t *curr = NULL, *prev = NULL;
    new = (dlnode_t *) safe_malloc (sizeof (dlnode_t));
    assert (list != NULL && new != NULL);
    new->size = size;
    new->data = value;
    new->next = NULL;
    new->prev = NULL;
    if (list->head == NULL) {
        list->head = new;
        list->foot = new;
    } else {
        curr = list->head;
        while ((curr != NULL) && (cmp_fptr (new->data, curr->data) > 0)) {
            prev = curr;
            curr = curr->next;
        }
        if (curr == NULL) {
            list->foot = new;
        } else {
            curr->prev = new;
        }
        if (prev == NULL) {
            list->head = new;
        } else {
            prev->next = new;
        }
        new->next = curr;
        new->prev = prev;
    }
    list->list_len++;
    list->size += size;
    list->size += sizeof (dlnode_t);
}

void*
find_element (dlist_t *list, void *value,
                            int (*cmp_fptr) (const void*, const void*))
{
    dlnode_t *curr = NULL;
    int result = 0;
    curr = list->head;
    while (curr != NULL) {
        result = cmp_fptr (curr->data, value);
        if (result == 0) {
            return (curr->data);
        }
        curr = curr->next;
    }
    return (NULL);
}

void*
remove_element(dlist_t* list,void* val,
            int (*cmp_fptr)(const void*,const void*))
{
	dlnode_t *curr = NULL, *prev = NULL;
    void* item;
    curr = list->head;
    while( curr != NULL && cmp_fptr (curr->data,val)!=0 ) {
        prev = curr;
        curr = curr->next;
    }

    /* not found return NULL */
    if(curr==NULL) return NULL;

    list->size -= curr->size;
    list->size -= sizeof(dlnode_t);

    if(prev==NULL) {
        list->head = curr->next;
        if(list->head) list->head->prev = NULL;
        item = curr->data;
        free(curr);
        if(list->head == NULL) list->foot = NULL;
    } else {
        item = curr->data;
        prev->next = curr->next;
        if(curr->next==NULL) list->foot = prev;
        else {
            prev->next->prev = prev;
        }
        free(curr);
    }
    list->list_len--;
    return item;
}

void*
remove_random (dlist_t *list)
{
	dlnode_t *curr = NULL, *prev = NULL;
    void* item;
	assert (list != NULL);
	int i = rand() % list->list_len;
    curr = list->head;
	while(i) {
		prev = curr;
		curr = curr->next;
        i--;
	}

    list->size -= curr->size;
    list->size -= sizeof(dlnode_t);

    if(prev==NULL) {
        list->head = curr->next;
        if(list->head) list->head->prev = NULL;
        item = curr->data;
        free(curr);
        if(list->head == NULL) list->foot = NULL;
    } else {
        item = curr->data;
        prev->next = curr->next;
        if(curr->next==NULL) list->foot = prev;
        else {
            prev->next->prev = prev;
        }
        free(curr);
    }
    list->list_len--;
    return item;
}


