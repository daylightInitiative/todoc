
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

typedef void (*CommandFunc)(int argc, char *argv[], void *data);

typedef struct {
    const char *name;
    const char *description;
    CommandFunc func;
} Command;

struct Node *create_task(int argc, char *argv[], void *data);
void cmd_help(int argc, char *argv[], void *data);
void cmd_exit(int argc, char *argv[], void *data);
void list_tasks(int argc, char *argv[], void *data);
void add_task(int argc, char *argv[], void *data);

void cmd_exit(int argc, char *argv[], void *data) {
    (void)data;
    printf("Exiting.\n");
    exit(0);
}

Command commands[] = {
    {"help", "Show available commands", cmd_help},
    {"add", "Add a new task", add_task},
    {"list", "Show all tasks", list_tasks},
    {"exit", "Exit the program", cmd_exit},
    {"quit", "Exit the program", cmd_exit},
    {NULL, NULL, NULL} // sentinel
};


void cmd_help(int argc, char *argv[], void *data) {
    printf("\n=== Available Commands ===\n\n");

    for (int i = 0; commands[i].name != NULL; i++) {
        printf("  %-10s - %s\n", commands[i].name, commands[i].description);
    }

    printf("\nType a command and press Enter.\n\n");
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


void list_tasks(int argc, char *argv[], void *data) {

    struct List *lp = (struct List *)data;
    echo_list(lp);

}



struct Node *create_task(int argc, char *argv[], void *data) {
    struct List *lp = (struct List *)data;
    struct Node *node = insert_node(lp);
    if (!node) {
        fprintf(stderr, "Failed to insert new task.\n");
        return NULL;
    }

    char title_buffer[MAX_NAME_LEN] = {0};
    char description_buffer[MAX_DESC_LEN] = {0};

    if (argc > 1 && argv[1] != NULL) {
        strncpy(title_buffer, argv[1], MAX_NAME_LEN - 1);
    } else {
        printf("Enter task title: ");
        if (!fgets(title_buffer, sizeof(title_buffer), stdin)) {
            fprintf(stderr, "Failed to read title.\n");
            remove_node(lp, node);
            return NULL;
        }
        title_buffer[strcspn(title_buffer, "\n")] = '\0'; // Trim newline
    }

    strncpy(node->title, title_buffer, MAX_NAME_LEN - 1);
    node->title[MAX_NAME_LEN - 1] = '\0';

    if (argc > 2 && argv[2] != NULL) {
        strncpy(description_buffer, argv[2], MAX_DESC_LEN - 1);
    } else {
        printf("Enter task description: ");
        if (!fgets(description_buffer, sizeof(description_buffer), stdin)) {
            fprintf(stderr, "Failed to read description.\n");
            remove_node(lp, node);
            return NULL;
        }
        description_buffer[strcspn(description_buffer, "\n")] = '\0'; // Trim newline
    }

    strncpy(node->description, description_buffer, MAX_DESC_LEN - 1);
    node->description[MAX_DESC_LEN - 1] = '\0';

    node->task_status = UNCOMPLETED_TASK;

    printf("Task '%s' added successfully.\n", node->title);

    return node;
}


struct Node *find_task_by_id(struct List *lp, int id) {
    struct Node *current = lp->head;
    while (current != NULL) {
        if (current->id == id) return current;
        current = current->next;
    }
    return NULL;
}

void add_task(int argc, char *argv[], void *data) {
    create_task(argc, argv, data);
}

#define MAX_TOKENS 16

void run_command(char *input, void *context) {
    char *argv[MAX_TOKENS];
    int argc = 0;

    char *token = strtok(input, " \t");
    while (token && argc < MAX_TOKENS) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    if (argc == 0)
        return;

    for (int i = 0; commands[i].name != NULL; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].func(argc, argv, context);
            return;
        }
    }

    printf("Unknown command: %s\n", argv[0]);
}



int main() {
    struct List *lp = create_list();

    // Set up SIGINT handler
    signal(SIGINT, handle_sigint);

    char buffer[2048];
    memset(buffer, 0, sizeof(buffer));

    while (!stop) {
        printf(">>> ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = '\0'; // trim newline

            if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0)
                break;

            run_command(buffer, lp);

        } else {
            break; // fgets failed (stdin closed or EOF)
        }

        memset(buffer, 0, sizeof(buffer));
    }

    delete_list(lp);

    return 0;
}