#include "aufgabe_4_3_queue.h"

void queue_init(queue_t *queue)
{
   queue->posRead = 0;
   queue->size    = 0;
   
   pthread_mutex_init(&(queue->mutex), NULL);
   pthread_cond_init(&(queue->canRead), NULL);
   pthread_cond_init(&(queue->canWrite), NULL);
}

void queue_put(queue_t *queue, char b)
{
   pthread_mutex_lock(&(queue->mutex));
   
   if (queue->size == QUEUE_MAX_SIZE)
   {
	   pthread_cond_wait(&(queue->canWrite), &(queue->mutex));
   }
   /*do the fifo stuff*/
   int posWrite = (queue->posRead + queue->size) % QUEUE_MAX_SIZE;
   queue->size++;
   queue->buffer[posWrite] = b;
   pthread_cond_signal(&(queue->canRead));
   pthread_mutex_unlock(&(queue->mutex));
}

char queue_get(queue_t *queue)
{
   pthread_mutex_lock(&(queue->mutex));
   
   if (queue->size == 0)
   {
	   pthread_cond_wait(&(queue->canRead), &(queue->mutex));
   }
   /*do the fifo stuff*/
   char value     = queue->buffer[queue->posRead];
   queue->posRead = (queue->posRead + 1) % QUEUE_MAX_SIZE;
   queue->size--;
   pthread_cond_signal(&(queue->canWrite));
   pthread_mutex_unlock(&(queue->mutex));

   return value;
}
