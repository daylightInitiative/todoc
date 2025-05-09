
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <signal.h>
#include <unistd.h>

// create main function
// create linked list
// add command prompt to get input
// create commands and parse

#include "list.h"

typedef void (*CommandFunc)(void *);

typedef struct {
    const char *name;
    const char *description;
    CommandFunc func;
} Command;

struct Node *create_task(void *data);
void cmd_help(void *data);
void cmd_exit(void *data);
void list_tasks(void *data);
void add_task(void *data);

void cmd_exit(void *data) {
    (void)data;
    printf("Exiting.\n");
    exit(0);
}

Command commands[] = {
    {"help", "Show available commands", cmd_help},
    {"add", "Add a new task", add_task},
    {"tasks", "Show all tasks", list_tasks},
    {"exit", "Exit the program", cmd_exit},
    {NULL, NULL, NULL} // sentinel
};


void cmd_help(void *data) {
    printf("\n=== Available Commands ===\n\n");

    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %-10s - %s\n", commands[i].name, commands[i].description);
    }

    printf("\nType a command and press Enter.\n\n");
}

void run_command(const char *input, void *context) {
    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            commands[i].func(context);  // pass list or app context here
            return;
        }
    }
    printf("Unknown command: %s\n", input);
}


#define UNCOMPLETED_TASK  0
#define COMPLETED_TASK    1




volatile sig_atomic_t stop = 0;

void handle_sigint(int sig) {
    (void)sig;
    stop = 1;

    // Close stdin to force fgets to return
    fclose(stdin);

    write(STDOUT_FILENO, "\n[!] SIGINT received. Exiting...\n", 33);
}


void list_tasks(void *data) {

    struct List *lp = (struct List *)data;
    echo_list(lp);

}



struct Node *create_task(void *data) {
    struct List *lp = (struct List *)data;
    struct Node *node = insert_node(lp);
    if (!node) {
        fprintf(stderr, "Failed to insert new task.\n");
        return NULL;
    }

    char title_buffer[MAX_NAME_LEN];
    char description_buffer[MAX_DESC_LEN];

    memset(title_buffer, 0, sizeof(title_buffer));
    memset(description_buffer, 0, sizeof(description_buffer));

    printf("Enter task title: ");
    if (!fgets(title_buffer, sizeof(title_buffer), stdin)) {
        fprintf(stderr, "Failed to read title.\n");
        remove_node(lp, node);
        return NULL;
    }
    title_buffer[strcspn(title_buffer, "\n")] = '\0'; // Trim newline
    strncpy(node->title, title_buffer, MAX_NAME_LEN - 1);
    node->title[MAX_NAME_LEN - 1] = '\0';

    printf("Enter task description: ");
    if (!fgets(description_buffer, sizeof(description_buffer), stdin)) {
        fprintf(stderr, "Failed to read description.\n");
        remove_node(lp, node);
        return NULL;
    }
    description_buffer[strcspn(description_buffer, "\n")] = '\0'; // Trim newline
    strncpy(node->description, description_buffer, MAX_DESC_LEN - 1);
    node->description[MAX_DESC_LEN - 1] = '\0';

    node->task_status = UNCOMPLETED_TASK;

    printf("Task '%s' added successfully.\n", node->title);

    return node;
}

void add_task(void *data) {
    create_task(data);
}

void complete_task(void *data) {

}

int main() {
    struct List *lp = create_list();

    // Set up SIGINT handler
    signal(SIGINT, handle_sigint);

    char buffer[2048];
    memset(buffer, '\0', sizeof(buffer));

    while (!stop) {

        printf(">>> ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            //printf("%s\n", buffer);



            buffer[strcspn(buffer, "\n")] = '\0'; // trim newline

            if (strcmp(buffer, "exit") == 0)
                break;

            run_command(buffer, lp);


        } else {
            break; // fgets failed (stdin closed or EOF)
        }

        memset(buffer, '\0', sizeof(buffer));
    }

    delete_list(lp);

    return 0;
}