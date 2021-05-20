#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_INPUT_LENGTH 2048
#define MAX_NUM_OF_PARAMETERS 255

bool getInput(char **cmd) {
    static char input[MAX_INPUT_LENGTH];

    printf("myshell:%s\n> ", getcwd(NULL, 0));

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return false;
    }

    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }

    char *token;
    token = strtok(input, " ");  // NOLINT
    int i = 0;
    while (token != NULL) {
        cmd[i] = token;
        token  = strtok(NULL, " ");  // NOLINT
        i++;
        if (i == MAX_NUM_OF_PARAMETERS - 1) {
            break;
        }
    }
    cmd[i] = NULL;

    return true;
}

int main(void) {
    char *cmd[MAX_NUM_OF_PARAMETERS];
    bool  finished = false;

    while (!finished) {
        if (getInput(cmd)) {
            if (cmd[0] == NULL) {
                // ignore empty line
            } else {
                pid_t childPid;
                switch (childPid = fork()) {
                case -1:
                    perror("fork() failed");
                    return EXIT_FAILURE;
                case 0:
                    execvp(cmd[0], cmd);
                    fprintf(stderr, "executing %s failed: %s\n", cmd[0],
                            strerror(errno));
                    return EXIT_FAILURE;
                default:
                    waitpid(childPid, NULL, 0);
                    break;
                }
            }
        } else {
            finished = true;
        }
    }

    return EXIT_SUCCESS;
}
