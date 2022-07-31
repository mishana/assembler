//
// Created by misha on 30/07/2022.
//

#include "linkedlist.h"

#include<stdio.h>
#include<stdlib.h>
#include "errors.h"


/* A generic linked list node */
typedef struct node_t {
    void *data;
    struct node_t *next;
} *Node;

struct list_t {
    Node head;
    list_eq leq;
    list_copy lcopy;
    list_free lfree;
};

/**
 * It creates a list.
 *
 * @param leq a function that compares two elements of the list.
 * @param lcopy a function that takes a pointer to a list element and returns a pointer to a copy of that element.
 * @param lfree a function that frees the data in the list
 */
List listCreate(list_eq leq, list_copy lcopy, list_free lfree) {
    List l = (List) malloc(sizeof(*l));
    if (!l)
        memoryAllocationError();

    l->head = NULL;
    l->leq = leq;
    l->lcopy = lcopy;
    l->lfree = lfree;

    return l;
}

/**
 * Adds a node at the beginning of a Linked List.
 *
 * @param l a pointer to the List to insert into
 * @param new_data the data to be inserted into the l
 * @param copy_data a function that returns a copy of a data item.
 */
ListResult listInsertFirst(List l, void *new_data) {
    if (!l || !new_data)
        return LIST_NULL_ARGUMENT;

    Node new_node = (Node) malloc(sizeof(*new_node));
    if (!new_node)
        /* It's a function that prints an error message and exits the program. */
        memoryAllocationError();

    new_node->data = l->lcopy(new_data);
    new_node->next = l->head;

    l->head = new_node;
    return LIST_SUCCESS;
}

/**
 * Adds a node at the end of a Linked List.
 *
 * @param l a pointer to the List to insert into
 * @param new_data the data to be inserted into the l
 */
ListResult listAppend(List l, void *new_data) {
    if (!l || !new_data)
        return LIST_NULL_ARGUMENT;

    Node new_node = (Node) malloc(sizeof(*new_node));
    if (new_node == NULL)
        memoryAllocationError();

    new_node->data = l->lcopy(new_data);
    new_node->next = NULL;

    /* If the l is empty, then the new node is the only node in the l. */
    if (!l->head) {
        l->head = new_node;
    } else {
        /* Iterating through the l until it reaches the last node. */
        Node it;
        for (it = l->head; it->next; it = it->next);
        it->next = new_node;
    }
    return LIST_SUCCESS;
}

/**
 * It finds the first element in the list that matches the given element.
 *
 * @param l The list to search through
 * @param to_find the value to find in the list
 * @param found a pointer to a copy of the found value
 */
ListResult listFindAndCopy(List l, void *to_find, void **found) {
    if (!l || !to_find)
        return LIST_NULL_ARGUMENT;

    for (Node it = l->head; it; it = it->next) {
        /* Comparing the data in the node to the data we are looking for. */
        if (l->leq(it->data, to_find) == 0) {
            *found = l->lcopy(it->data);
            return LIST_FOUND;
        }
    }
    return LIST_NOT_FOUND;
}

/**
 * It returns the length of the list.
 *
 * @param l a pointer to a list
 */
int length(List l) {
    if (!l->head)
        return 0;

    int count = 1;
    Node ptr = l->head;
    while (ptr->next) {
        ptr = ptr->next;
        count++;
    }
    return count;
}

/**
 * It destroys the list - frees the memory.
 *
 * @param l A pointer to a List to destroy.
 */
void listDestroy(List l) {
    /* If the list is empty, there's nothing to destroy. */
    if (!l->head)
        return;

    /* Freeing the memory allocated for the nodes in the list. */
    while (l->head) {
        Node to_delete = l->head;
        l->head = l->head->next;
        l->lfree(to_delete->data);
        free(to_delete);
    }
    free(l);
}
