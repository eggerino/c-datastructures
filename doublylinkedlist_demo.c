#include <stdio.h>

#define DOUBLYLINKEDLIST_DEFINITION
#include "doublylinkedlist.h"

void print_info(const DoublyLinkedList* list) {
    printf("length: %u\n", list->length);

    DoublyLinkedListNode* node = list->head;
    size_t i = 0;
    while (node) {
        printf("- [%u]\t%s\n", i++, (char*)node->item);
        node = node->next;
    }
}

int main(void) {
    DoublyLinkedList* list = dll_new();
    print_info(list);

    dll_append(list, (void*)", ");
    print_info(list);
    dll_append(list, (void*)"World!");
    print_info(list);
    dll_prepend(list, (void*)"Hello");
    print_info(list);

    DoublyLinkedListNode* commaNode = list->head->next;
    dll_node_prepend(commaNode, (void*)"1");
    print_info(list);
    dll_node_append(commaNode, (void*)"2");
    print_info(list);

    dll_remove(list->head);
    print_info(list);
    dll_remove(list->tail);
    print_info(list);

    dll_remove(commaNode);
    print_info(list);

    dll_delete(list);
    return 0;
}
