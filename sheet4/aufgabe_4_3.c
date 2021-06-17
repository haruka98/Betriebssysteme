#define _XOPEN_SOURCE 500
#define _POSIX_C_SOURCE 199506L

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "aufgabe_4_3_queue.h"

#define NUM_THREADS 7
#define NUM_ITEMS 10

queue_t fifo;

volatile int numOfThreads;

pthread_mutex_t m;

void *printing(void *param)
{
   while (numOfThreads > 0) {
      printf("%d: size of queue %d\n", numOfThreads, fifo.size);
      usleep(100000);
   }

   return NULL;
}

void *produce(void *param)
{
   int *counter = (int *)param;

   for (int i = 0; i < NUM_ITEMS; i++) {
      queue_put(&fifo, i);
      (*counter)++;
      usleep(200000 + (rand() % 200000));
   }
   pthread_mutex_lock(&m);
   numOfThreads--;
   pthread_mutex_unlock(&m);

   return NULL;
}

void *consume(void *param)
{
   int *counter = (int *)param;

   for (int i = 0; i < NUM_ITEMS; i++) {
      queue_get(&fifo);
      (*counter)++;
      usleep(200000 + (rand() % 200000));
   }
   pthread_mutex_lock(&m);
   numOfThreads--;
   pthread_mutex_unlock(&m);

   return NULL;
}

int main(void)
{
   printf("entering main()\n");

   time_t t;
   time(&t);
   srand((unsigned int)t);

   pthread_mutex_init(&m, NULL);

   numOfThreads = 2 * NUM_THREADS;

   /*init fifo*/
   queue_init(&fifo);

   int counterDataWrite[NUM_THREADS];
   int counterDataRead[NUM_THREADS];

   pthread_t printer;

   pthread_t producers[NUM_THREADS];
   pthread_t consumers[NUM_THREADS];

   printf("starting printer\n");

   if (pthread_create(&printer, NULL, printing, NULL)) {
      printf("creating thread failed");
      return EXIT_FAILURE;
   }

   usleep(50000);

   printf("starting producers\n");

   /*starting producers*/
   for (int p = 0; p < NUM_THREADS; p++) {
      counterDataWrite[p] = 0;
      if (pthread_create(&producers[p], NULL, produce,
                         (void *)&counterDataWrite[p])) {
         printf("creating thread failed");
         return EXIT_FAILURE;
      }
   }

   sleep(1);

   printf("Starting consumers\n");

   /*starting consumers*/
   for (int c = 0; c < NUM_THREADS; c++) {
      counterDataRead[c] = 0;
      if (pthread_create(&consumers[c], NULL, consume,
                         (void *)&counterDataRead[c])) {
         printf("creating thread failed");
         return EXIT_FAILURE;
      }
   }

   /* waiting for printer thread */
   pthread_join(printer, NULL);

   /* collecting producers */
   for (int p = 0; p < NUM_THREADS; p++) {
      pthread_join(producers[p], NULL);
   }

   /* collecting consumers */
   for (int c = 0; c < NUM_THREADS; c++) {
      pthread_join(consumers[c], NULL);
   }

   printf("printing statistics\n");

   int count = 0;
   for (int t = 0; t < NUM_THREADS; t++) {
      count += counterDataRead[t];
      printf("producer %d wrote %d chars\n", t, counterDataWrite[t]);
   }
   printf("total %d chars\n", count);

   count = 0;
   for (int t = 0; t < NUM_THREADS; t++) {
      count += counterDataRead[t];
      printf("consumer %d read %d chars\n", t, counterDataRead[t]);
   }
   printf("total %d chars\n", count);

   printf("leaving main()\n");

   return EXIT_SUCCESS;
}
