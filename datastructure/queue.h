#ifndef DATASTRUCTURES_LISTS_QUEUE_H_
#define DATASTRUCTURES_LISTS_QUEUE_H_

#include "LinkedList.h"


// MARK: DATA TYPES

// Queues are used to access a linked list in a controlled manner.
struct Queue
{
    /* MEMBER VARIABLES */
    // A reference to the embedded LinkedList.
    struct LinkedList list;

    /* PUBLIC MEMBER METHODS */
    // The push function adds a node to the end of the chain.
    void (*push)(struct Queue *queue, void *data, unsigned long size);
    // The peek function retrieves the data from the front of the list.
    void * (*peek)(struct Queue *queue);
    // The pop function removes the first item in the list.
    void (*pop)(struct Queue *queue);

};

struct Queue queue_constructor(void);
void queue_destructor(struct Queue *queue);

#endif /* DATASTRUCTURES_LISTS_QUEUE_H_ */
