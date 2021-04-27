#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *executable = "bob";
static const char *progname = "roberta";

int main(int argc, char *argv[])
{
    char *path = getenv("PATH");

    if (path != NULL) {
        printf("searching for executable %s in path %s\n", executable, path);
    } else {
        fprintf(stderr, "error: variable PATH is unknown\n");
        return EXIT_FAILURE;
    }

    if (argc >= 2) {
        execlp(executable, progname, argv[1], NULL);
    } else {
        execlp(executable, progname, NULL);
    }

    // this line is only reached if an error occured
    fprintf(stderr, "%s: error executing bob - %s\n", argv[0], strerror(errno));

    return EXIT_FAILURE;
}
