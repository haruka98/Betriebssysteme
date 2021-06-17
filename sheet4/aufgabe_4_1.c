#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 200809L

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

const key_t SEM_KEY_FORKS = 0x12345;
int         forks;

bool acquire(int left, int right)
{
   struct sembuf sops[2];

   sops[0].sem_num = left;
   sops[0].sem_op  = -1;
   sops[0].sem_flg = IPC_NOWAIT | SEM_UNDO;
   sops[1].sem_num = right;
   sops[1].sem_op  = -1;
   sops[1].sem_flg = IPC_NOWAIT | SEM_UNDO;

   if (semop(forks, sops, sizeof(sops) / sizeof(struct sembuf)) == -1) {
      if (errno == EAGAIN) {
         return false;  // acquire failed
      } else {
         fprintf(stderr, "acquire - semop(%d,%d) failed: %s\n", left, right,
                 strerror(errno));
         exit(EXIT_FAILURE);
      }
   }

   return true;  // acquire successful
}

void release(int left, int right)
{
   struct sembuf sops[2];

   sops[0].sem_num = left;
   sops[0].sem_op  = +1;
   sops[0].sem_flg = SEM_UNDO;
   sops[1].sem_num = right;
   sops[1].sem_op  = +1;
   sops[1].sem_flg = SEM_UNDO;

   if (semop(forks, sops, sizeof(sops) / sizeof(struct sembuf)) == -1) {
      fprintf(stderr, "release - semop(%d,%d) failed: %s\n", left, right,
              strerror(errno));
      exit(EXIT_FAILURE);
   }
}

int left(int i)
{
   return i; /* left fork */
}

int right(int i)
{
   return (i + 1) % N; /* right fork */
}

void take_forks(int who)
{
   while (!acquire(left(who), right(who))) {
      sleep(1);
   }
}

void put_forks(int who)
{
   release(left(who), right(who));
}

void think(int who)
{
   printf("philosopher %d: thinking...\n", who);
   sleep(2);
}

void eat(int who)
{
   printf("philosopher %d: eating...\n", who);
   sleep(6);
}

void philosopher(int who)
{
   while (true) {
      think(who);
      take_forks(who);
      eat(who);
      put_forks(who);
   }
}

int main(int argc, char **argv)
{
   if (argc != 2) {
      fprintf(stderr, "usage: %s <number>\n", argv[0]);
      return EXIT_FAILURE;
   }

   int phil = atoi(argv[1]);

   if (phil < 0 || phil >= N) {
      fprintf(stderr, "usage: phil number must be between 0 and %d\n", N - 1);
      return EXIT_FAILURE;
   }

   printf("try to create semaphore %d\n", SEM_KEY_FORKS);
   if ((forks = semget(SEM_KEY_FORKS, N, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
      if (errno == EEXIST) {
         printf("semaphore %d already existing\n", SEM_KEY_FORKS);
         if ((forks = semget(SEM_KEY_FORKS, N, 0600)) == -1) {
            fprintf(stderr, "semget() failed: %s\n", strerror(errno));
            return EXIT_FAILURE;
         }
      } else {
         fprintf(stderr, "semget() failed: %s\n", strerror(errno));
         return EXIT_FAILURE;
      }
   } else {
      printf("semaphore %d is created: initializing...\n", SEM_KEY_FORKS);
      for (int i = 0; i < N; i++) {
         if (semctl(forks, i, SETVAL, (int)1) == -1) {
            fprintf(stderr, "semctl(%d) failed: %s\n", i, strerror(errno));
            return EXIT_FAILURE;
         }
      }
   }

   philosopher(phil);

   return EXIT_SUCCESS;
}
