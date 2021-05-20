#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void) {
    pid_t childPid;
    int fd[2];
    pipe(fd);
    switch (childPid = fork()) {
    case -1:
        perror("fork() failed");
        return EXIT_FAILURE;
    case 0:
        dup2(fd[1], STDOUT_FILENO);
        execlp("ls", "ls", NULL);
        perror("execlp(ls)");
        break;
    default:
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        execlp("wc", "wc", "-l", NULL);
        perror("execl(ls)");
        break;
    }

    return EXIT_SUCCESS;
}
