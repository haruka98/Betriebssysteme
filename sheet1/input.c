#include <stdio.h>
#include <stdlib.h>

#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 255
#define USER_FILENO 10

int main(int argc, char *argv[]) {
    int fds[] = {STDIN_FILENO, USER_FILENO};

    for (int i = 0; i < sizeof(fds) / sizeof(fds[0]); i++) {
        char    buffer[MAX_BUFFER_SIZE];
        ssize_t nbytes;
        int     fd = fds[i];

        printf("reading from file descriptor %d\n", fd);
        while ((nbytes = read(fd, buffer, sizeof(buffer))) > 0) {
            printf("(%ld)[%.*s]\n", nbytes, (int)nbytes, buffer);
        }

        if (nbytes == -1) {
            fprintf(stderr, "file descriptor %d is not ready - %s\n", fd,
                    strerror(errno));
        }
    }

    return EXIT_SUCCESS;
}
