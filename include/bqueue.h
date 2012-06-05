/**
 * @brief Blocking queue: a queue for which, when a thread calls @a bqueue_dequeue on an empty queue, this thread is blocked until another thread calls \a bqueue_queue to queue an element.
 * @file bqueue.h
 * @author Michel SIMATIC
 * @date  14/04/2012
 */

#ifndef _BQUEUE_H_
#define _BQUEUE_H_

#include <semaphore.h>
#include "list.h"

/** 
 * @brief Data structure holding a queue
 */
typedef struct{
  t_list *list;   /**< List holding the different values in the queue */
  sem_t  readSem; /**< Semaphore used to determine how many values are present in \a list */
} t_bqueue;

/**
 * @brief Creates a new bqueue
 * @return Pointer to the new bqueue
 */
t_bqueue *bqueue_new();

/**
 * @brief Removes the first element of @a aBQueue. In case \a aBQueue is empty, blocks the executing thread until an other thread adds an element to \a aBQueue.
 * @param[in] aBQueue Bqueue to work on
 * @return The first element of \a aBQueue
 */
void *bqueue_dequeue(t_bqueue *aBQueue);

/**
 * @brief Enqueues element @a anElt at the end of queue @a aBQueue
 * @param[in] aBQueue Bqueue to work on
 * @param[in] anElt Element to add to \a aBQueue
 */
void bqueue_enqueue(t_bqueue *aBQueue, void *anElt);

/**
 * @brief Do several enqueue of elements of @a list in @a aBQueue
 * @param[in] aBQueue The pointer on the t_bqueue wich is bouned to grow
 * @param[in] list The pointer on the list to enqueue
 */
void bqueue_extend(t_bqueue *aBQueue, t_list *list);

/**
 * @brief Frees @a aBQueue
 * @param[in] aBQueue List to work on
 * @warning 
 * <ol>
 * <li>If elements of @a aBQueue were pointers to allocated structures, these elements are not freed.</li>
 * <li>Freeing a \a bqueue that other threads are currently blocked on (in \a bqueue_dequeue) produces undefined behavior.</li>
 * </ol>
 */
void bqueue_free(t_bqueue *aBQueue);

#endif /* _BQUEUE_H_ */
