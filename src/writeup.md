# Todoc writeup
Technical details on LL (Lessons learned) and new information I learned while creating this project

I found this interesting as I haven't had that much experience with concating strings with the C preprocessor.
```c
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define RESET   "\033[0m"

#define STATUS_UNCOMPLETED  RED "Uncompleted" RESET
#define STATUS_COMPLETED    GREEN "Completed" RESET
```

Function pointers were something new to me, especially ones that pass uniform arguments.
```c
typedef void (*CommandFunc)(int argc, char *argv[], void *data);

typedef struct {
    const char *name;
    const char *description;
    CommandFunc func;
} Command;
```

I have seen other gnu utils do this before, and I learned what a `sentinel` is.
```c
Command commands[] = {
    {"help", "Show available commands", cmd_help},
    {"add", "Add a new task", add_task},
    {"list", "Show all tasks", list_tasks},
    {"toggle","Toggles the task with the given task id", toggle_task_complete},
    {"exit", "Exit the program", cmd_exit},
    {"quit", "Exit the program", cmd_exit},
    {NULL, NULL, NULL} // sentinel
};
```

Force closing stdin to get fgets to stop blocking, this was interesting. A problem i've faced before on my chat client.
```c
void handle_sigint(int sig) {
    (void)sig;
    stop = 1;

    // Close stdin to force fgets to return
    fclose(stdin);

    write(STDOUT_FILENO, "\n[!] SIGINT received. Exiting...\n", 33);
}
```
