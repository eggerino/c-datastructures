#include <stdio.h>

#define HASHTABLE_DEFINITION
#include "hashtable.h"

int main(void) {
    HashTable* ht = ht_new();

    char* hello = "hello";
    char* world = "world";
    char* some = "some";
    char* value = "value";

    void* old_value;
    ht_set(ht, "hello", hello, &old_value);
    ht_set(ht, "world", world, &old_value);

    printf("length: %d\tcapacity: %d\n", ht->length, ht->capacity);
    HashTableIterator iter = ht_iterate(ht);
    while (ht_next(&iter)) {
        printf("%s: %s\n", iter.key, iter.value);
    }

    ht_set(ht, "hello", some, &old_value);
    printf("old_value: %s\n", old_value);

    printf("length: %d\tcapacity: %d\n", ht->length, ht->capacity);
    iter = ht_iterate(ht);
    while (ht_next(&iter)) {
        printf("%s: %s\n", iter.key, iter.value);
    }

    ht_set(ht, "1", value, &old_value);
    ht_set(ht, "2", value, &old_value);
    ht_set(ht, "3", value, &old_value);
    ht_set(ht, "4", value, &old_value);
    ht_set(ht, "5", value, &old_value);
    ht_set(ht, "6", value, &old_value);

    printf("length: %d\tcapacity: %d\n", ht->length, ht->capacity);
    iter = ht_iterate(ht);
    while (ht_next(&iter)) {
        printf("%s: %s\n", iter.key, iter.value);
    }

    char* val = (char*)ht_get(ht, "world");
    printf("value at key 'world': %s\n", val);

    ht_remove(ht, "1");
    ht_remove(ht, "2");
    ht_remove(ht, "3");

    printf("length: %d\tcapacity: %d\n", ht->length, ht->capacity);
    iter = ht_iterate(ht);
    while (ht_next(&iter)) {
        printf("%s: %s\n", iter.key, iter.value);
    }

    ht_delete(ht);
    return 0;
}
