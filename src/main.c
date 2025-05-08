
#include <stdio.h>
#include <stdlib.h>

// create main function
// create linked list
// add command prompt to get input
// create commands and parse

#include "list.h"

#define UNCOMPLETED_TASK  0
#define COMPLETED_TASK    1

struct Node *create_task(List *lp, char task_name[], task_description[]) {

    // create a test task
    struct Node *node1 = insert_node(lp);
    // ids are handled internally
    strncpy(node1->title, task_name, MAX_NAME_LEN - 1); 
    node1->title[MAX_NAME_LEN - 1] = '\0'; // Ensure null termination

    strncpy(node1->description, task_name, MAX_NAME_LEN - 1); 
    node1->description[MAX_DESC_LEN - 1] = '\0'; // Ensure null termination

    node1->task_status = UNCOMPLETED_TASK;
    return node1;
}

int main(int argc, char **argv)
{
    struct List *LP = create_list();

    


    delete_list(LP);

    return 0;
}