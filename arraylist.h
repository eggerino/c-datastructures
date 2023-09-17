#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_LIST(T) ArrayListOf_##T
#define AL_NEW(T, initial_capacity) al_of_##T##_new(initial_capacity)
#define AL_DELETE(T, list) al_of_##T##_delete(list)
#define AL_ADD(T, list, item) al_of_##T##_add(list, item)
#define AL_REMOVE(T, list) al_of_##T##_remove(list)

#define ARRAY_LIST_DECLARATION(T)                             \
    typedef struct ArrayListOf_##T {                          \
        size_t length;                                        \
        size_t capacity;                                      \
        T* data;                                              \
    } ArrayListOf_##T;                                        \
                                                              \
    ARRAY_LIST(T) * al_of_##T##_new(size_t initial_capacity); \
    void al_of_##T##_delete(ARRAY_LIST(T) * list);            \
    uint8_t al_of_##T##_add(ARRAY_LIST(T) * list, T item);    \
    T al_of_##T##_remove(ARRAY_LIST(T) * list);

#define ARRAY_LIST_DEFINITION(T)                                                                                      \
    ARRAY_LIST(T) * al_of_##T##_new(size_t initial_capacity) {                                                        \
        assert(initial_capacity > 0 && "Initial capacity must be greater than zero.");                                \
                                                                                                                      \
        ARRAY_LIST(T)* list = (ARRAY_LIST(T)*)malloc(sizeof(ARRAY_LIST(T)));                                          \
        T* data = (T*)malloc(sizeof(T) * initial_capacity);                                                           \
                                                                                                                      \
        if (list == NULL || data == NULL) {                                                                           \
            free(list);                                                                                               \
            free(data);                                                                                               \
            return NULL;                                                                                              \
        }                                                                                                             \
                                                                                                                      \
        list->length = 0;                                                                                             \
        list->capacity = initial_capacity;                                                                            \
        list->data = data;                                                                                            \
        return list;                                                                                                  \
    }                                                                                                                 \
                                                                                                                      \
    void al_of_##T##_delete(ARRAY_LIST(T) * list) {                                                                   \
        assert(list != NULL && "List pointer must not be null. Consider checking for null on creation of the list."); \
                                                                                                                      \
        free(list->data);                                                                                             \
        free(list);                                                                                                   \
    }                                                                                                                 \
                                                                                                                      \
    uint8_t al_of_##T##_add(ARRAY_LIST(T) * list, T item) {                                                           \
        assert(list != NULL && "List pointer must not be null");                                                      \
                                                                                                                      \
        if (list->length == list->capacity) {                                                                         \
            size_t capacity = list->capacity + 1;                                                                     \
            T* data = (T*)malloc(sizeof(T) * capacity);                                                               \
                                                                                                                      \
            if (data == NULL) {                                                                                       \
                return 1;                                                                                             \
            }                                                                                                         \
                                                                                                                      \
            memcpy(data, list->data, list->length * sizeof(T));                                                       \
            free(list->data);                                                                                         \
                                                                                                                      \
            list->capacity = capacity;                                                                                \
            list->data = data;                                                                                        \
        }                                                                                                             \
                                                                                                                      \
        list->data[list->length] = item;                                                                              \
        ++list->length;                                                                                               \
                                                                                                                      \
        return 0;                                                                                                     \
    }                                                                                                                 \
                                                                                                                      \
    T al_of_##T##_remove(ARRAY_LIST(T) * list) {                                                                      \
        assert(list != NULL && "List pointer must not be null");                                                      \
        assert(list->length > 0 && "List is already empty. Cannot remove any items");                                 \
                                                                                                                      \
        --list->length;                                                                                               \
        return list->data[list->length];                                                                              \
    }

#endif
