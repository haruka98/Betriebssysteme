#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    const char *name;

    switch (argc) {
        case 1:
            name = "unknown fellow";
            break;
        case 2:
            name = argv[1];
            break;
        default:
            fprintf(stderr, "usage: %s [name]\n", argv[0]);
            return EXIT_FAILURE;
    }

    printf("Hello %s, my name is %s\n", name, argv[0]);

    return EXIT_SUCCESS;
}
