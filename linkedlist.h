//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_LINKEDLIST_H
#define ASSEMBLER_LINKEDLIST_H

/* A linked list node */
typedef struct node {
    // Any data type can be stored in this node
    void *data;
    struct node *next;
} Node;

typedef struct list {
    Node *head;
} List;

void initList(List *pList);

void insertFirst(List *pList, void *new_data, void *(*copy_data)(void *));

void insertLast(List *pList, void *new_data, void *(*copy_data)(void *));

int length(List list);

void destroyList(List *plist, void (*destroy_data)(void *));

#endif //ASSEMBLER_LINKEDLIST_H
