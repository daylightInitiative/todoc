
#ifndef TODO_LIST_H
#define TODO_LIST_H

#define MAX_NAME_LEN 256 // bytes
#define MAX_DESC_LEN 1048 // bytes
#define MAX_PRIORITY_LEN 32 // bytes

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0m"

#define STATUS_UNCOMPLETED  RED "Uncompleted" RESET
#define STATUS_COMPLETED    GREEN "Completed" RESET

struct Node {

    char title[MAX_NAME_LEN];
    char description[MAX_DESC_LEN];

    int priority;
    int id;

    int task_status;

    struct Node *next;
    struct Node *prev;
};

struct List {
 
    struct Node *head;
    struct Node *tail;

    unsigned int next_id; // used for uniquely identifying tasks like fg or kill does
    unsigned int capacity;
};


struct List *create_list(void);
void delete_list(struct List *lp);
void echo_list(struct List *lp);

struct Node *insert_node(struct List *lp);
void remove_node(struct List *lp, struct Node *node);

#endif
