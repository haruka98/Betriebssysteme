#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>
#include <errno.h>

#define MAX_BUFFER_SIZE 255
#define USER_FILENO 10

int main(int argc, char *argv[])
{
    int fds[] = {STDOUT_FILENO, STDERR_FILENO, USER_FILENO};

    for (int i = 0; i < sizeof(fds)/sizeof(fds[0]); i++) {
        char string[MAX_BUFFER_SIZE];
        int fd = fds[i];
        int nbytes = snprintf(string, sizeof(string),
                              "writing to file descriptor %d\n", fd);
        if (write(fd, string, nbytes) != nbytes) {
            fprintf(stderr, "file descriptor %d is not ready - %s\n", fd,
                    strerror(errno));
        }
    }

    return EXIT_SUCCESS;
}
