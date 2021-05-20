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
    pid_t parentPid;
    switch (childPid = fork()) {
    case -1:
        perror("fork() failed");
        return EXIT_FAILURE;
    case 0:
        parentPid = getppid();
        printf("child: my pid = %d\n", getpid());
        printf("child: parent pid = %d\n", getppid());
        sleep(2);
        printf("child (after sleep): my pid = %d\n", getpid());
        printf("child (after sleep): paretn pid = %d\n", getppid());
        if(parentPid != getppid()) {
            printf("Is orphan process.\n");
        }
        break;
    default:
        printf("parent: my pid = %d\n", getpid());
        printf("parent: pid of child = %d\n", childPid);
        printf("parent: parent pid = %d\n", getppid());
        break;
    }

    printf("process %d is exiting\n", getpid());
    return EXIT_SUCCESS;
}
