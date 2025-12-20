
#include "list.h"
#include <stdlib.h>
#include <string.h>

Node* create_node(void* data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;

    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void free_node(Node* node) {
    if (!node) {
        return;
    }

    if (node->next) {
        free_node(node->next);
    }

    free(node);
}

/*
LINKED LIST
*/

LinkedList* create_linked_list() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));
    if (list) {
        list->size = 0;
        list->first = NULL;
    }
    return list;
}

void free_linked_list(LinkedList* list) {
    if (!list) return;

    list->size = 0;
    free_node(list->first);
    list->first = NULL;

    free(list);
}


int linked_list_add(LinkedList *list, void *element) {
    if (!list) return 1;

    Node *new_node = create_node(element);
    if (!new_node) return 1;   // malloc failed

    if (!list->first) {
        list->first = new_node;
    } else {
        Node *last = list->first;
        while (last->next)
            last = last->next;

        last->next = new_node;
    }

    list->size++;
    return 0;
}


/*
get the node at index "index" in the linked list "list". Returns NUll if this node does not exist
*/
Node* linked_list_get(LinkedList* list, int index) {
    if (!list) return NULL;
    
    int step = 0; // steps into the list

    Node* node = list->first;

    if (!node) return NULL;

    while (node->next && step < index)
    {
        node = node->next;
        step++;
    }
    
    if (step != index) return NULL;
    return node;
}