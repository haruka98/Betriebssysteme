#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    pid_t childPid;
    int return_value;
    int fd;
    switch (childPid = fork()) {
    case -1:
        perror("fork() failed");
        return EXIT_FAILURE;
    case 0:
        fd = open("/dev/null", O_WRONLY);
        dup2(fd, STDOUT_FILENO);
        close(fd);
        execl("dice", "dice", NULL);
        perror("execl(dice)");
        break;
    default:
        waitpid(childPid, &return_value, 0);
        if(WIFEXITED(return_value)) {
            printf("Roll (dicer): %d\n", WEXITSTATUS(return_value));
        }
        break;
    }

    printf("process %d is exiting\n", getpid());
    return EXIT_SUCCESS;
}
