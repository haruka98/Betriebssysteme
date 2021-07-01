#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TMP_FILE_TEMPLATE "/tmp/bs_tmpfile_XXXXXX"

int main(void) {
    int  fd;
    char filename[] = TMP_FILE_TEMPLATE;

    if ((fd = mkstemp(filename)) == -1) {
        fprintf(stderr, "failed to create %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("File %s is visible.\nPress [Enter] to continue.\n", filename);
    getchar();  // wait for ENTER

    if (unlink(filename) == -1) {
        fprintf(stderr, "failed to unlink %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("File %s is invisible.\nPress [Enter] to continue.\n", filename);
    getchar();  // wait for ENTER

    close(fd);

    return EXIT_SUCCESS;
}
