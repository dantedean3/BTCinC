#include <stdio.h>
#include "list_t.h"

int main() {
    list_t my_list = list_create();

    // Add values 1, 2, 3 to the list
    my_list = list_add(my_list, (void *)1);
    my_list = list_add(my_list, (void *)2);
    my_list = list_add(my_list, (void *)3);

    printf("After adding elements:\n");
    list_t temp = my_list;
    while (temp) {
        printf("Value: %ld\n", (long)temp[0]);
        temp = temp[1];
    }

    my_list = list_remove(my_list, (void *)2);
    printf("\nAfter removing 2:\n");
    temp = my_list;
    while (temp) {
        printf("Value: %ld\n", (long)temp[0]);
        temp = temp[1];
    }

    list_free(my_list);
    return 0;
}
