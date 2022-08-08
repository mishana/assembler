//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_LINKEDLIST_H
#define ASSEMBLER_LINKEDLIST_H


#include <stdbool.h>

typedef int (*list_eq)(const void *a, const void *b);

typedef void *(*list_copy)(const void *);

typedef void (*list_free)(void *);

typedef struct node_t *Node;
typedef struct list_t *List;

/** possible return values */
typedef enum {
    LIST_SUCCESS, LIST_NULL_ARGUMENT, LIST_NOT_FOUND, LIST_FOUND
} ListResult;

List listCreate(list_eq leq, list_copy lcopy, list_free lfree);

List listCopy(List l);

List listCopyFromNode(List l, Node n);

ListResult listInsertFirst(List l, void *new_data);

ListResult listAppend(List l, void *new_data);

ListResult listFind(List l, void *to_find, void **found);

int listLength(List l);

void listDestroy(List l);

const void *nodeGetData(Node n);

Node listHead(List l);

Node nodeGetNext(Node n);

#endif //ASSEMBLER_LINKEDLIST_H
