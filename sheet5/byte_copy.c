#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int src;
    int dest;
    int bufSize;

    if (argc != 2) {
        fprintf(stderr, "usage: byte_copy buf_size");
        return EXIT_FAILURE;
    }
    if ((bufSize = atoi(argv[1])) <= 0) {
        fprintf(stderr, "buf_size must be greater than zero");
        return EXIT_FAILURE;
    }

    if ((src = open("test.orig", O_RDONLY)) == -1) {
        perror("failed to open source file");
        return EXIT_FAILURE;
    }
    if ((dest = open("test.copy", O_CREAT | O_TRUNC | O_WRONLY, 0600)) == -1) {
        perror("failed to create destination file");
        return EXIT_FAILURE;
    }

    char    buf[bufSize];  // NOLINT
    ssize_t bytes;
    bool    finished = false;

    while (!finished) {
        bytes = read(src, buf, sizeof(buf));
        if (bytes == -1) {
            perror("failed to read from file");
            finished = true;
        } else if (bytes == 0) {
            finished = true;
        } else {
            if (write(dest, buf, bytes) != bytes) {
                perror("failed to write from file");
                finished = true;
            }
        }
    }

    if (close(src) == -1) {
        perror("failed to close source file");
        return EXIT_FAILURE;
    }
    if (close(dest) == -1) {
        perror("failed to close destination file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
