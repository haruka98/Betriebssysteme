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

#define N 5
#define LEFT(i) ((N + i - 1) % N)
#define RIGHT(i) ((i + 1) % N)

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int             state[N];
pthread_mutex_t lock;
pthread_mutex_t s[N];

void acquire(pthread_mutex_t * m) { pthread_mutex_lock(m); }

void release(pthread_mutex_t * m) { pthread_mutex_unlock(m); }

void check_forks(int who)
{
   if (state[who] == HUNGRY &&
      state[LEFT(who)] != EATING &&
      state[RIGHT(who)] != EATING) {
      state[who] = EATING;
      release(&s[who]);
   }
}

void take_forks(int who)
{
   acquire(&lock);
   printf("philosopher %d: take_forks...\n", who);
   state[who] = HUNGRY;
   check_forks(who);
   release(&lock);
   acquire(&s[who]);
}

void put_forks(int who)
{
   acquire(&lock);
   printf("philosopher %d: put_forks...\n", who);
   state[who] = THINKING;
   check_forks(LEFT(who));
   check_forks(RIGHT(who));
   release(&lock);
}

void think(int who)
{
   printf("philosopher %d: thinking...\n", who);
   sleep(2);
}

void eat(int who)
{
   printf("philosopher %d: eating...\n", who);
   sleep(4);
}

void *philosopher(void *arg)
{
   int who = *((int *)arg);

   while (true) {
      think(who);
      take_forks(who);
      eat(who);
      put_forks(who);
   }
   return NULL;
}

int main(int argc, char **argv)
{
   pthread_mutex_init(&lock, NULL);
   for (int i = 0; i < N; i++) {
      state[i] = THINKING;
      pthread_mutex_init(&s[i], NULL);
      pthread_mutex_lock(&s[i]);
   }

   pthread_t t[N];
   for (int i = 0; i < N; i++) {
      int *who = malloc(sizeof(int));
      *who     = i;
      pthread_create(&t[i], NULL, philosopher, who);
   }

   for (int i = 0; i < N; i++) {
      pthread_join(t[i], NULL);
   }

   return EXIT_SUCCESS;
}
