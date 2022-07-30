//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_LINKEDLIST_H
#define ASSEMBLER_LINKEDLIST_H


typedef int (*list_eq)(const void *a, const void *b);

typedef void *(*list_copy)(const void *);

typedef void (*list_free)(void *);

typedef struct list_t *List;

List listCreate(list_eq leq, list_copy lcopy, list_free lfree);

void listInsertFirst(List pList, void *new_data);

void listAppend(List l, void *new_data);

void *listFind(List l, void *to_find);

int length(List l);

void listDestroy(List l);

#endif //ASSEMBLER_LINKEDLIST_H
