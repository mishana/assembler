//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_LINKEDLIST_H
#define ASSEMBLER_LINKEDLIST_H


typedef int (*list_eq)(const void *a, const void *b);

typedef void *(*list_copy)(const void *);

typedef void (*list_free)(void *);

typedef struct list_t *List;

/** possible return values */
typedef enum {
    LIST_SUCCESS, LIST_NULL_ARGUMENT, LIST_NOT_FOUND, LIST_FOUND
} ListResult;

List listCreate(list_eq leq, list_copy lcopy, list_free lfree);

ListResult listInsertFirst(List l, void *new_data);

ListResult listAppend(List l, void *new_data);

ListResult listFindAndCopy(List l, void *to_find, void **found);

int length(List l);

void listDestroy(List l);

#endif //ASSEMBLER_LINKEDLIST_H
