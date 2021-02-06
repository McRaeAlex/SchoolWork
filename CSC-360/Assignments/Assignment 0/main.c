#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

struct node {
    int val;
    struct node *next;
    struct node *prev;
};

struct node *Node_New() {
    struct node *temp = malloc(sizeof(struct node));
    temp->val = 0; // This happens anyways but whatever
    temp->next = NULL;
    temp->prev = NULL;
    return temp;
}

void LinkedList_AddToEnd(struct node* current, struct node* val) {
    while(current != NULL) {
        if (current->next == NULL) {
            break;
        }
        current = current->next;
    }
    current->next = val;
}

void LinkedList_AddToFront(struct node* current, struct node* val) {
    while(current != NULL) {
        if (current->prev == NULL) {
            break;
        }
        current = current->prev;
    }
    current->prev = val;
}

void LinkedList_Display(struct node* head_of_list) {
    while(head_of_list!= NULL) {
        printf("%d\n", head_of_list->val);

        if (head_of_list->next == NULL) {
            break; // return early if end of list
        }
        head_of_list = head_of_list->next;
    }
}

int main() {
    struct node* first = Node_New();
    LinkedList_Display(first);
}