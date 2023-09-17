#include <stdio.h>

#include "arraylist.h"

ARRAY_LIST_DECLARATION(double)
ARRAY_LIST_DEFINITION(double)

void print_info(ARRAY_LIST(double)* list) {
    printf("length: %u\tcapacity: %u\n", list->length, list->capacity);
    for (size_t i = 0; i < list->length; ++i) {
        printf("- [%u]\t%f\n", i, list->data[i]);
    }
}

int main(void) {
    ARRAY_LIST(double)* list = AL_NEW(double, 1);
    print_info(list);

    AL_ADD(double, list, 1.0);
    print_info(list);
    AL_ADD(double, list, 2.0);
    print_info(list);
    AL_ADD(double, list, 3.0);
    print_info(list);
    AL_ADD(double, list, 4.0);
    print_info(list);

    AL_REMOVE(double, list);
    print_info(list);
    AL_REMOVE(double, list);
    print_info(list);
    AL_REMOVE(double, list);
    print_info(list);
    AL_REMOVE(double, list);
    print_info(list);

    AL_DELETE(double, list);

    return 0;
}