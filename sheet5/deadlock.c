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
#include <sys/wait.h>
#include <unistd.h>

#define FILENAME "test.dat"
#define STR "----------\n"

void setLock(int fd, bool lock, int offset) {
    struct flock fl;

    fl.l_type   = lock ? F_WRLCK : F_UNLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = offset;
    fl.l_len    = 1;

    if (fcntl(fd, F_SETLKW, &fl) == -1) {
        fprintf(stderr, "fcntl failed on %d: %s\n", offset, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void workWithFile(char *filename, char c, int offset1, int offset2) {
    int fd;

    if ((fd = open(filename, O_WRONLY)) == -1) {
        fprintf(stderr, "open file %s failed: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    sleep(1);
    setLock(fd, true, offset1);
    sleep(1);
    setLock(fd, true, offset2);
    lseek(fd, offset1, SEEK_SET);
    write(fd, &c, sizeof(c));
    printf("process %d writes '%c' at position %d\n", getpid(), c, offset1);
    sleep(1);
    lseek(fd, offset2, SEEK_SET);
    write(fd, &c, sizeof(c));
    printf("process %d writes '%c' at position %d\n", getpid(), c, offset2);
    setLock(fd, false, offset1);
    setLock(fd, false, offset2);

    close(fd);
}

int main(void) {
    int fd;

    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    if ((fd = open(FILENAME, O_WRONLY | O_CREAT, mode)) == -1) {
        fprintf(stderr, "open file %s failed: %s\n", FILENAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    write(fd, STR, strlen(STR));

    close(fd);

    pid_t pid;
    if ((pid = fork()) == 0) {
        workWithFile(FILENAME, 'A', 1, 8);
    } else {
        workWithFile(FILENAME, 'B', 1, 8);
        waitpid(pid, NULL, 0);
    }

    return EXIT_SUCCESS;
}
