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

    switch (childPid = fork()) {
    case -1:
        perror("fork() failed");
        return EXIT_FAILURE;
    case 0:
        printf("child: my pid = %d\n", getpid());
        printf("child: parent pid = %d\n", getppid());
        printf("Press [Enter] to continue.\n");
        getchar();  // wait for ENTER
        break;
    default:
        printf("parent: my pid = %d\n", getpid());
        printf("parent: pid of child = %d\n", childPid);
        printf("parent: parent pid = %d\n", getppid());
        waitpid(childPid, NULL, 0);
        break;
    }

    printf("process %d is exiting\n", getpid());
    return EXIT_SUCCESS;
}
