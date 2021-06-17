#ifndef AUFGABE_4_3_QUEUE_H
#define AUFGABE_4_3_QUEUE_H

#include <pthread.h>

#define QUEUE_MAX_SIZE 5

typedef struct {
   /*should be used if consumer must wait*/
   pthread_cond_t canRead;
   /*should be used if producer must wait*/
   pthread_cond_t canWrite;
   /*should be used when entering critical section*/
   pthread_mutex_t mutex;

   /*queue buffer*/
   char buffer[QUEUE_MAX_SIZE];

   /*current position for reading*/
   int posRead;
   /*current elements in queue*/
   int size;
} queue_t;

/**
 * Initializes queue.
 * @param queue pointer to queue struct
 */
void queue_init(queue_t *queue);

/**
 * Writes a character into queue. Function
 * blocks until character can be written.
 *
 * @param queue pointer to initialized queue struct
 * @param b     character
 */
void queue_put(queue_t *queue, char b);

/**
 * Reads a character from queue. Function
 * blocks until character can be read.
 *
 * @param queue pointer to initialized queue struct
 * @return b    character
 */
char queue_get(queue_t *queue);

#endif
