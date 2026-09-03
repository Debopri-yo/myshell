#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#define HISTORY_FILE "history.txt"
#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_HISTORY 5
void get_history_path(char *buffer, size_t size) {
    char path[MAX_LINE];
    ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
    if (len == -1) {
        perror("readlink");
        exit(1);
    }
    path[len] = '\0';
    char *dir = dirname(path);
    snprintf(buffer, size, "%s/%s", dir, HISTORY_FILE);
}
void saveHistory(char history[][MAX_LINE], int history_count, char *path){    
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Could not open history file");
        return;
    }
    int start = (history_count > MAX_HISTORY) ? (history_count - MAX_HISTORY) : 0;
    for (int i=start;i< history_count;i++){
        int index = i % MAX_HISTORY;
        fprintf(file, "%d. %s\n", i + 1, history[index]);
    }
    fclose(file);
}
void loadHistory(char history[][MAX_LINE], int *history_count, char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        return;
    }
    char line[MAX_LINE];
    int last_num = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        char *dot = strchr(line, '.');
        int num;
        char *cmd;
        if (dot && dot[1] == ' ') {
            num = atoi(line);
            cmd = dot + 2;
        } else {
            // malformed/legacy line, skip numbering assumption
            num = last_num + 1;
            cmd = line;
        }

        int index = (num - 1) % MAX_HISTORY;
        strncpy(history[index], cmd, MAX_LINE - 1);
        history[index][MAX_LINE - 1] = '\0';

        if (num > last_num) last_num = num;
    }
    *history_count = last_num;
    fclose(file);
}
void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \t\n");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \t\n");
    }
}

int main() {
    char input[MAX_LINE];
    char *args[MAX_ARGS];
    char history[MAX_HISTORY][MAX_LINE];
    pid_t pid;
    int status;
    int history_count = 0;
    char cwd[MAX_LINE];
    char path[MAX_LINE];
    get_history_path(path, sizeof(path));
    loadHistory(history, &history_count, path);
    while (1) {
        if (!(getcwd(cwd, sizeof(cwd)))) {
            perror("getcwd failed");
            continue;
        }
        printf("myshell:~%s:", cwd);
        if (!fgets(input, MAX_LINE, stdin)) {
           saveHistory(history, history_count, path);
            break;
        }
	//!number functionality start
        if (input[0] == '!' && input[1] != '\0') {
            int valid = 1;
            for (int i = 1; input[i] != '\n' && input[i] != '\0'; i++) {
                if (!isdigit((unsigned char)input[i])) {
                    valid = 0;
                    break;
                }
            }

            if (valid) {
                int cmd_num = atoi(input + 1);
                int min_cmd = (history_count > MAX_HISTORY) ? history_count - MAX_HISTORY + 1 : 1;
                int max_cmd = history_count;

                if (cmd_num >= min_cmd && cmd_num <= max_cmd) {
                    int index = (cmd_num - 1) % MAX_HISTORY;
                    strncpy(input, history[index], MAX_LINE - 1);
                    input[MAX_LINE - 1] = '\0'; // ensure null termination
                    goto process_input;
                } else {
                    printf("No such command in history\n");
		    continue;
                }
            }
        }
	//!number functionality end
        process_input:
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        } // remove \n from input

        int index = history_count % MAX_HISTORY;
        strcpy(history[index], input);
        history_count++;

        parse_input(input, args);

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0 || strcmp(args[0], "quit") == 0) {
            saveHistory(history, history_count, path);
            break;
        }

        if (strcmp(args[0], "history") == 0) {
            int start = (history_count > MAX_HISTORY) ? (history_count - MAX_HISTORY) : 0;
            for (int i = start; i < history_count; i++) {
                int index = i % MAX_HISTORY;
                printf("%d. %s\n", i + 1, history[index]);
            }
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else if (chdir(args[1]) == -1) {
                perror("chdir failed");
            }
            continue;
        }

        pid = fork();
        if (pid < 0) {
            perror("fork failed");
        } else if (pid == 0) {
            if (execvp(args[0], args) < 0) {
                perror("exec failed");
                exit(1);
            }
        } else {
            waitpid(pid, &status, 0);
        }
    }

    return 0;
}
