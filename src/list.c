#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list.h"

struct List *create_list() {
    struct List *lp = NULL;
    if ((lp = (struct List *)calloc(1, sizeof(struct List))) == NULL) {
        fprintf(stderr, "Failure to allocate sufficient resources\n");
        return NULL;
    }

    lp->capacity = 0;
    lp->next_id = 1;
    
    lp->head = NULL;
    lp->tail = NULL;

    return lp;
}

void echo_list(struct List *lp) {
    if (lp->capacity == 0) {
        printf("\nNo current tasks.\n");
        return;
    }

    printf("\n=== Task List ===\n\n");

    struct Node *current = lp->head;
    while (current != NULL) {
        printf("ID         : %d\n", current->id);
        printf("Title      : %s\n", current->title);
        printf("Description: %s\n", current->description);
        printf("Priority   : %d\n", current->priority);
        printf("Status     : %s\n", 
            current->task_status == 0 ? "Uncompleted" : "Completed");
        printf("---------------------------\n");

        current = current->next;
    }
}

void delete_list(struct List *lp) {
    while (lp->head != NULL) {
        remove_node(lp, lp->head);
    }
    free(lp);  // Free the list structure itself
}

struct Node *insert_node(struct List *lp) {
    struct Node *node = calloc(1, sizeof(struct Node));
    if (!node) {
        fprintf(stderr, "Failure to allocate sufficient resources\n");
        return NULL;
    }

    node->id = lp->next_id++;

    if (lp->head == NULL) {
        lp->head = node;
        lp->tail = node;
        node->prev = NULL;
        node->next = NULL;
    } else {
        lp->tail->next = node;
        node->prev = lp->tail;
        node->next = NULL;
        lp->tail = node;
    }

    lp->capacity++;
    return node;
}

void remove_node(struct List *lp, struct Node *node) {
    if (lp->head != node) {
        if (node->prev != NULL) {
            node->prev->next = node->next;
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
        }

        if (node->next == NULL) {
            lp->tail = node->prev;
        }
    } else {
        if (lp->head->next != NULL) {
            lp->head = lp->head->next;
            lp->head->prev = NULL;
            if (lp->head->next == NULL) {
                lp->tail = NULL;
            }
        } else {
            lp->head = NULL;
            lp->tail = NULL;
        }
    }

    node->prev = NULL;
    node->next = NULL;
    lp->capacity--;
    free(node);
}
