#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 199506L

#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFSIZE 20

// foward declarations
void *write1(void *ptr);
void *write2(void *ptr);

// buffer
int  count;
char array[BUFSIZE];

pthread_mutex_t mutex;

int main(void)
{
   // Initialize buffer
   count = 0;
   memset(array, 0, sizeof(array));

   pthread_mutex_init(&mutex, NULL);

   // Create writing threads
   pthread_t thread1, thread2;
   pthread_create(&thread1, NULL, write1, NULL);
   pthread_create(&thread2, NULL, write2, NULL);

   // Wait for threads to complete. Prevents process and all threads
   // from terminating before they are completed.
   pthread_join(thread1, NULL);
   pthread_join(thread2, NULL);

   // Output
   write(STDOUT_FILENO, array, count);
   write(STDOUT_FILENO, "\n", 1);

   return EXIT_SUCCESS;
}

void do_something() { usleep(60); }

// Writes 'A's into array
void *write1(void *ptr)
{
   bool finished = false;
   while (!finished) {
      pthread_mutex_lock(&mutex);
      if (count >= BUFSIZE) {
         finished = true;
      } else {
         array[count] = array[count] + 'A';
         count++;
      }
      pthread_mutex_unlock(&mutex);
      do_something();
   }
   return NULL;
}

// Writes 'B's into array
void *write2(void *ptr)
{
   bool finished = false;
   while (!finished) {
      pthread_mutex_lock(&mutex);
      if (count >= BUFSIZE) {
         finished = true;
      } else {
         array[count] = array[count] + 'B';
         count++;
      }
      pthread_mutex_unlock(&mutex);
      do_something();
   }
   return NULL;
}
