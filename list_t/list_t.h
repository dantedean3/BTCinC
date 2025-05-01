#ifndef LIST_T_H
#define LIST_T_H

#include <stddef.h>

typedef void **list_t;

list_t list_create();
list_t list_add(list_t list, void *data);
list_t list_remove(list_t list, void *data);
void list_free(list_t list);

#endif
