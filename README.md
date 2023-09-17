# About

A collection of reuseable data structures implemented in C.
This is mainly a learning exercise for me to get more familiar with the data structures.

# Usage

Every datastructure is one single self contained header file.
Copy the header file(s) of the desired data sturcture(s) into your project's include directory.
Check out the corresponding demo source file on how the use a specific data structure.

# Data structures

## Array list (vector)

One of the advantages of the data structure is, that every entry in the array list is located next to each other in memory.
This makes it easy to optimize for performance, since the entire block of memory can be loaded into the cache without derefencing pointers while e.g. iterating over the data.
To achieve a generic and reuseable implementation of an array list a macro-based aproach is used.

The header file only contains a few macros for a generic declaration, definition and usage of the data structure.

In one source file include the header and use the definition macro with the desired inner type.
```C
#include "arraylist.h"
ARRAY_LIST_DEFINITION(int)      // for an array list of integers
ARRAY_LIST_DEFINITION(void*)    // for an array list of pointers
```
The implementation of every specific type will be expanded by these macros into this translation unit and compiled by your compiler.

## Doubly linked list

It is implemented as a [stb-style library](https://github.com/nothings/stb).
In one source file include the header with the definition macro.
```C
#define DOUBLYLINKDEDLIST_DEFINITION
#include "doublylinkedlist.h"
```
The implementation of the doubly linked list will be included into this translation unit and compiled by your compiler.

## Hash table

The mein idea of the implementation is from this [Blog](https://benhoyt.com/writings/hash-table-in-c/) ([Repo](https://github.com/benhoyt/ht/tree/master)).
It is also implemented as a [stb-style library](https://github.com/nothings/stb).
In one source file include the header with the definition macro.
```C
#define HASHTABLE_DEFINITION
#include "hashtable.h"
```
The implementation of the hashtable will be included into this translation unit and compiled by your compiler.

## Other

Some data structures can be derived from already implementated data structures.

To use a queue one can simply use the doubly linked list and just use `dll_append` to enque, `dll_remove(list->head)` to deque and `list->length` to check for an empty queue.

For a stack one can use the doubly linked list in a similar manner, but with `dll_remove(list->tail)` to pop from the stack. It is also possible (and probably recommanded) to use a array list instead.
With a nicely conditioned capacity the amount of heap allocations can be minified significantly.