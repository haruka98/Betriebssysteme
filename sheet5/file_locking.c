#define _XOPEN_SOURCE 500

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FILENAME "test.dat"
#define STR "-------------------------------\n"
#define COUNT 5
int offset[] = {1, 7, 13, 19, 25};

void writeFile(char *filename, char *str) {
    int p  = 0;
    int fd = open(filename, O_WRONLY);

    while (true) {
        lseek(fd, offset[p], SEEK_SET);
        for (int i = 0; i < COUNT; i++) {
            write(fd, &str[i], sizeof(str[i]));
            usleep(500000);
        }
        usleep(500000);
        p = (p + 1) % 5;
    }
}

int main(void) {
    int    fd;
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    if ((fd = open(FILENAME, O_WRONLY | O_CREAT, mode)) == -1) {
        fprintf(stderr, "open file %s failed: %s\n", FILENAME, strerror(errno));
        exit(EXIT_FAILURE);
    }
    write(fd, STR, strlen(STR));
    close(fd);

    if (fork() == 0) {
        writeFile(FILENAME, "Homer");
    } else {
        writeFile(FILENAME, "Marge");
    }

    return EXIT_SUCCESS;
}
