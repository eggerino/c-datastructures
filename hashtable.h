#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stddef.h>

typedef struct HashTableItem {
    char* key;
    void* value;
    size_t hash;
} HashTableItem;

typedef struct HashTable {
    size_t capacity;
    size_t length;
    HashTableItem* items;
} HashTable;

typedef struct HashTableIterator {
    const char* key;
    void* value;
    const HashTable* table;
    size_t index_after;
} HashTableIterator;

HashTable* ht_create(void);
void ht_destroy(HashTable* ht);

void* ht_get(const HashTable* ht, const char* key);
void* ht_remove(HashTable* ht, const char* key);
void* ht_set(HashTable* ht, const char* key, void* value, void** old_value);

HashTableIterator ht_iterate(const HashTable* ht);
int ht_next(HashTableIterator* iter);

#endif  // HASHTABLE_H_

#ifdef HASHTABLE_DEFINITION

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifndef HASHTABLE_INITIAL_CAPACITY
#define HASHTABLE_INITIAL_CAPACITY 11
#endif  // HASHTABLE_INITIAL_CAPACITY

#ifndef HASHTABLE_CAPACITY_INCREASE_RATE
#define HASHTABLE_CAPACITY_INCREASE_RATE 1.5
#endif  // HASHTABLE_CAPACITY_INCREASE_RATE

#ifndef HASHTABLE_RATIO
#define HASHTABLE_RATIO 0.7
#endif  // HASHTABLE_RATIO

#ifndef HASHTABLE_HASH_FUNCTION
#define HASHTABLE_HASH_FUNCTION ht_fnv_1a
#endif  // HASHTABLE_HASH_FUNCTION

static int ht_find_index(const HashTable* ht, const char* key, size_t hash, size_t* index);
static int ht_increase(HashTable* ht);
static void ht_insert(HashTable* ht, char* key, void* value, size_t hash);
static size_t ht_fnv_1a(const char* key);

HashTable* ht_create(void) {
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    // Ensure all keys and values in the array are NULL pointers
    HashTableItem* items = (HashTableItem*)calloc(HASHTABLE_INITIAL_CAPACITY, sizeof(HashTableItem));
    if (ht == NULL || items == NULL) {
        free(ht);
        free(items);
        return NULL;
    }

    ht->capacity = HASHTABLE_INITIAL_CAPACITY;
    ht->length = 0;
    ht->items = items;
    return ht;
}

void ht_destroy(HashTable* ht) {
    if (ht == NULL) {
        return;
    }

    assert(ht->items != NULL && "Every hashtable must have an allocated items array.");
    for (size_t i = 0; i < ht->capacity; ++i) {
        free(ht->items[i].key);
    }

    free(ht->items);
    free(ht);
}

void* ht_get(const HashTable* ht, const char* key) {
    size_t hash = HASHTABLE_HASH_FUNCTION(key);

    size_t index;
    if (ht_find_index(ht, key, hash, &index)) {
        return NULL;
    }

    return ht->items[index].value;
}

void* ht_remove(HashTable* ht, const char* key) {
    size_t hash = HASHTABLE_HASH_FUNCTION(key);

    size_t index;
    if (ht_find_index(ht, key, hash, &index)) {
        return NULL;
    }

    free(ht->items[index].key);
    ht->items[index].key = NULL;
    --(ht->length);
    return ht->items[index].value;
}

void* ht_set(HashTable* ht, const char* key, void* value, void** old_value) {
    size_t hash = HASHTABLE_HASH_FUNCTION(key);

    // just overwrite the value if the key already exists
    // and return the old value so it can be freed by the consumer
    size_t index;
    if (!ht_find_index(ht, key, hash, &index)) {
        *old_value = ht->items[index].value;
        ht->items[index].value = value;
        return ht->items[index].key;
    }
    *old_value = NULL;

    double new_ratio = (ht->length + 1.0) / ht->capacity;
    if (new_ratio > HASHTABLE_RATIO) {
        if (ht_increase(ht)) {
            return NULL;
        }
    }

    char* key_copy = (char*)malloc(sizeof(char) * strlen(key) + 1);
    if (key_copy == NULL) {
        return NULL;
    }
    strcpy(key_copy, key);
    ht_insert(ht, key_copy, value, hash);
    ++(ht->length);
    return key_copy;
}

HashTableIterator ht_iterate(const HashTable* ht) {
    HashTableIterator iter = {0};
    iter.table = ht;
    return iter;
}

int ht_next(HashTableIterator* iter) {
    for (size_t index = iter->index_after; index < iter->table->capacity; ++index) {
        if (iter->table->items[index].key) {
            iter->key = iter->table->items[index].key;
            iter->value = iter->table->items[index].value;
            iter->index_after = index + 1;
            return 1;
        }
    }

    return 0;
}

static int ht_find_index(const HashTable* ht, const char* key, size_t hash, size_t* index) {
    *index = hash % (ht->capacity - 1);

    assert((ht->length < ht->capacity) && "At least one item in the hashtable must be null");
    while (ht->items[*index].key) {
        printf("%s\n", ht->items[*index].key);
        if (strcmp(ht->items[*index].key, key) == 0) {
            return 0;
        }

        // linear probe on index collision
        ++*index;
        // table wraps around and the end
        if (*index >= ht->capacity) {
            *index = 0;
        }
    }

    // key is not contained in the table
    return -1;
}

static int ht_increase(HashTable* ht) {
    size_t capacity = ht->capacity * HASHTABLE_CAPACITY_INCREASE_RATE;
    HashTableItem* items = (HashTableItem*)calloc(capacity, sizeof(HashTableItem));
    if (items == NULL) {
        return -1;
    }

    size_t old_capacity = ht->capacity;
    HashTableItem* old_items = ht->items;

    ht->capacity = capacity;
    ht->items = items;

    for (size_t i = 0; i < old_capacity; ++i) {
        if (old_items[i].key == NULL) {
            continue;
        }

        ht_insert(ht, old_items[i].key, old_items[i].value, old_items[i].hash);
    }

    return 0;
}

static void ht_insert(HashTable* ht, char* key, void* value, size_t hash) {
    size_t index = hash % (ht->capacity - 1);

    assert((ht->length < ht->capacity) && "Hashtable must not use all memory in the array");
    while (1) {
        if (ht->items[index].key == NULL) {
            ht->items[index].key = key;
            ht->items[index].value = value;
            ht->items[index].hash = hash;
            return;
        }

        ++index;
    }
}

static size_t ht_fnv_1a(const char* key) {
    size_t offset = 14695981039346656037UL;
    size_t prime = 1099511628211UL;

    size_t hash = offset;
    for (const char* c = key; *c; ++c) {
        hash ^= (size_t)(unsigned char)(*c);
        hash *= prime;
    }
    return hash;
}

#endif  // HASHTABLE_DEFINITION
