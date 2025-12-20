#ifndef LIST_H
#define LIST_H


typedef struct node {
    void* data;
    struct node* next;
} Node;

typedef struct linked_list {
    Node* first;
    int size;
} LinkedList;

Node* create_node(void* data);

void free_node(Node* node);

int linked_list_add(LinkedList* list,void* element);

Node* linked_list_get(LinkedList* list, int index);

LinkedList* create_linked_list();

void free_linked_list(LinkedList* list);

#endif