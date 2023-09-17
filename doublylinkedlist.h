#ifndef DOUBLYLINKEDLIST_H_
#define DOUBLYLINKEDLIST_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct DoublyLinkedListNode DoublyLinkedListNode;
typedef struct DoublyLinkedList DoublyLinkedList;

struct DoublyLinkedListNode {
    DoublyLinkedList* list;
    void* item;
    DoublyLinkedListNode* next;
    DoublyLinkedListNode* previous;
};

struct DoublyLinkedList {
    DoublyLinkedListNode* head;
    DoublyLinkedListNode* tail;
    size_t length;
};

DoublyLinkedList* dll_new(void);
void dll_delete(DoublyLinkedList* list);
uint8_t dll_prepend(DoublyLinkedList* list, void* item);
uint8_t dll_append(DoublyLinkedList* list, void* item);
uint8_t dll_node_prepend(DoublyLinkedListNode* node, void* item);
uint8_t dll_node_append(DoublyLinkedListNode* node, void* item);
void* dll_remove(DoublyLinkedListNode* node);

#endif  // DOUBLYLINKEDLIST_H_

#ifdef DOUBLYLINKEDLIST_DEFINITION

#include <assert.h>
#include <stdint.h>

DoublyLinkedList* dll_new(void) {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        return NULL;
    }

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void dll_delete(DoublyLinkedList* list) {
    assert(list != NULL && "List must not be null. Consider checking for null after creation.");

    if (list->head != NULL) {
        assert(list->tail != NULL && "At least one node is in the list. The tail must not be null");

        DoublyLinkedListNode* node = list->head;
        while (node) {
            DoublyLinkedListNode* next = node->next;
            free(node);
            node = next;
        }
    }

    free(list);
}

uint8_t dll_prepend(DoublyLinkedList* list, void* item) {
    DoublyLinkedListNode* node = (DoublyLinkedListNode*)malloc(sizeof(DoublyLinkedListNode));

    if (node == NULL) {
        return 1;
    }

    ++list->length;
    node->item = item;
    node->list = list;
    node->previous = NULL;

    if (list->head == NULL) {
        // Empty list -> node is head and tail
        node->next = NULL;
        list->head = node;
        list->tail = node;
    } else {
        // At least one node (a head exists) -> prepend to head
        node->next = list->head;
        node->next->previous = node;
        list->head = node;
    }

    return 0;
}

uint8_t dll_append(DoublyLinkedList* list, void* item) {
    DoublyLinkedListNode* node = (DoublyLinkedListNode*)malloc(sizeof(DoublyLinkedListNode));

    if (node == NULL) {
        return 1;
    }

    ++list->length;
    node->item = item;
    node->list = list;
    node->next = NULL;

    if (list->tail == NULL) {
        // Empty list -> node is head and tail
        node->previous = NULL;
        list->head = node;
        list->tail = node;
    } else {
        // At least one node (a tail exists) -> append to tail
        node->previous = list->tail;
        node->previous->next = node;
        list->tail = node;
    }

    return 0;
}

uint8_t dll_node_prepend(DoublyLinkedListNode* node, void* item) {
    DoublyLinkedListNode* new_node = (DoublyLinkedListNode*)malloc(sizeof(DoublyLinkedListNode));

    if (new_node == NULL) {
        return 1;
    }

    ++node->list->length;
    new_node->item = item;
    new_node->list = node->list;

    if (node == node->list->head) {
        new_node->next = node;
        new_node->previous = NULL;
        node->previous = new_node;
        node->list->head = new_node;
    } else {
        new_node->next = node;
        new_node->previous = node->previous;
        node->previous = new_node;
        new_node->previous->next = new_node;
    }

    return 0;
}

uint8_t dll_node_append(DoublyLinkedListNode* node, void* item) {
    DoublyLinkedListNode* new_node = (DoublyLinkedListNode*)malloc(sizeof(DoublyLinkedListNode));

    if (new_node == NULL) {
        return 1;
    }

    ++node->list->length;
    new_node->item = item;
    new_node->list = node->list;

    if (node == node->list->tail) {
        new_node->previous = node;
        new_node->next = NULL;
        node->next = new_node;
        node->list->tail = new_node;
    } else {
        new_node->previous = node;
        new_node->next = node->next;
        node->next = new_node;
        new_node->next->previous = new_node;
    }

    return 0;
}

void* dll_remove(DoublyLinkedListNode* node) {
    assert(node->list->length > 0 && "List must not have length of zero.");

    --node->list->length;

    if (node->list->length == 0) {
        // node is the last item in the list, now it is empty.
        // Other cases are garanteed to have at least one more item in the list.
        // Meaning the head and tail pointers cannot point to the same node.
        node->list->head = NULL;
        node->list->tail = NULL;
    } else if (node == node->list->head) {
        // node was the head -> head shifts to second node
        node->next->previous = NULL;
        node->list->head = node->next;
    } else if (node == node->list->tail) {
        // node was the tail -> tail shifts to second last node
        node->previous->next = NULL;
        node->list->tail = node->previous;
    } else {
        node->previous->next = node->next;
        node->next->previous = node->previous;
    }

    void* item = node->item;
    free(node);
    return item;
}

#endif  // DOUBLYLINKEDLIST_DEFINITION
