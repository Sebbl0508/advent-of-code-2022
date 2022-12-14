#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct SQueueNode {
    void* data;
    struct SQueueNode* next;
    struct SQueueNode* prev;
} SQueueNode;

typedef struct SQueue {
    // Size of one item in bytes
    size_t item_size;

    // The node that is at the front of the queue
    // this will be handled next
    struct SQueueNode* head;

    // The node that is at the back of the queue
    // new items come after the current tail
    struct SQueueNode* tail;
} SQueue;

// Creates an empty queue
SQueue squeue_create(size_t item_size);

// Copies the item onto another node
void squeue_push(SQueue* q, void* item);

// Returns the next item in the queue
//
// This pointer is valid until the next call to pop
void* squeue_pop_front(SQueue* q);

// Returns if the queue is empty
bool squeue_is_empty(SQueue* q);

// Destroys all items in the queue and itself
void squeue_destroy(SQueue* q);

// Count number of items in the queue
size_t squeue_length(SQueue* q);
