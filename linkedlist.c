//
// Created by misha on 30/07/2022.
//

#include "linkedlist.h"

#include<stdio.h>
#include<stdlib.h>
#include "errors.h"


/**
 * `initList` initializes a pList by setting its head pointer to NULL
 *
 * @param pList a pointer to a List struct
 */
void initList(List *pList) {
    pList->head = NULL;
}

/**
 * Adds a node at the beginning of a Linked List.
 *
 * @param pList a pointer to the List to insert into
 * @param new_data the data to be inserted into the pList
 * @param copy_data a function that returns a copy of a data item.
 */
void insertFirst(List *pList, void *new_data, void* (*copy_data)(void*)) {
    Node* new_node = (Node*) malloc(sizeof(*new_node));
    if (!new_node)
        exit(MEMORY_ALLOCATION_ERROR);

    new_node->data = (*copy_data)(new_data);
    new_node->next = pList->head;

    pList->head = new_node;
}

/**
 * Adds a node at the end of a Linked List.
 *
 * @param pList a pointer to the List to insert into
 * @param new_data the data to be inserted into the pList
 * @param copy_data a function that returns a copy of a data item.
 */
void insertLast(List *pList, void *new_data, void* (*copy_data)(void*)) {
    Node* new_node = (Node*) malloc(sizeof(*new_node));
    if (new_node == NULL)
        exit(MEMORY_ALLOCATION_ERROR);

    new_node->data = (*copy_data)(new_data);
    new_node->next = NULL;

    /* If the pList is empty, then the new node is the only node in the pList. */
    if (!pList->head) {
        pList->head = new_node;
        return;
    }

    /* Iterating through the pList until it reaches the last node. */
    Node* it;
    for (it = pList->head; it->next; it = it->next);
    it->next = new_node;
}

/**
 * It returns the length of the list.
 *
 * @param list a List struct
 */
int length(List list) {
    if (!list.head)
        return 0;

    int count = 1;
    Node *ptr = list.head;
    while (ptr->next) {
        ptr = ptr->next;
        count++;
    }
    return count;
}

/**
 * It destroys the list - frees the memory.
 *
 * @param plist A pointer to a List to destroy.
 * @param destroy_data a function that destroys a data item.
 */
void destroyList(List *plist, void (*destroy_data)(void *)) {
    /* If the list is empty, there's nothing to destroy. */
    if (!plist->head)
        return;

    /* Freeing the memory allocated for the nodes in the list. */
    Node *ptr1, *ptr2;
    ptr1 = plist->head;
    while (ptr1) {
        ptr2 = ptr1;
        ptr1 = ptr1->next;
        (*destroy_data)(ptr2->data);
        free(ptr2);
    }
    plist->head = NULL;
}
