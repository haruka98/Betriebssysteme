#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    printf("Hello World!\n");
    printf("%d\n", getpid());

    getchar();

    return(EXIT_SUCCESS);
}
