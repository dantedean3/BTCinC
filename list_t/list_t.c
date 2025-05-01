#include <stdlib.h>
#include "list_t.h"

list_t list_create() {
    return NULL;
}

list_t list_add(list_t list, void *data) {
    list_t new_node = malloc(sizeof(void *) * 2);
    new_node[0] = data;
    new_node[1] = list;
    return new_node;
}

list_t list_remove(list_t list, void *data) {
    if (!list) return NULL;

    if (list[0] == data) {
        list_t next = list[1];
        free(list);
        return next;
    }

    list_t prev = list;
    list_t curr = list[1];

    while (curr) {
        if (curr[0] == data) {
            prev[1] = curr[1];
            free(curr);
            break;
        }
        prev = curr;
        curr = curr[1];
    }

    return list;
}

void list_free(list_t list) {
    while (list) {
        list_t next = list[1];
        free(list);
        list = next;
    }
}
