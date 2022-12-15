#include "utils/smpl_queue.h"

#include <string.h>

SQueue squeue_create(size_t item_size) {
    SQueue s = {0};
    s.item_size = item_size;
    s.head = NULL;
    s.tail = NULL;
    return s;
}

void squeue_push(SQueue* q, void* item) {
    SQueueNode* next_back_node = calloc(1, sizeof(SQueueNode));

    // Copy the item into the new item's data pointer memory
    next_back_node->data = calloc(1, q->item_size);
    memcpy(next_back_node->data, item, q->item_size);

    if(q->tail == NULL || q->head == NULL) {
        // This *always* happends ???
        q->tail = NULL;
    }

    if(q->tail == NULL && q->head == NULL) {
        q->tail = next_back_node;
        q->head = next_back_node;
    } else {
        // The new node's previous is the old tail
        if(q->head != q->tail) {
            next_back_node->prev = q->tail;
        }

        // The old tail's next node is the new node
        q->tail->next = next_back_node;

        // The new tail is the new node
        q->tail = next_back_node;
    }
}

void* squeue_pop_front(SQueue* q) {
    static void* item_cache = NULL;
    if(item_cache == NULL) {
        item_cache = calloc(1, q->item_size);
    }

    if(q->head == NULL) {
        return NULL;
    }

    memcpy(item_cache, q->head->data, q->item_size);

    SQueueNode* old_head = q->head;
    if(q->head == q->tail) {
        q->tail = NULL;
    }

    q->head = q->head->next;
    if(q->head != NULL)
        q->head->prev = NULL;

    free(old_head->data);
    free(old_head);
    return item_cache;
}

void squeue_destroy(SQueue* q) {
    SQueueNode* head = q->head;
    while(head != NULL) {
        SQueueNode* this_head = head;

        head = head->next;

        free(this_head->data);
        free(this_head);
    }
    memset(q, 0, sizeof(SQueue));
}

bool squeue_is_empty(SQueue* q) {
    return (q->head == NULL ? true : false);
}

size_t squeue_length(SQueue* q) {
    size_t len = 0;
    SQueueNode* cur_node = q->head;
    for(len = 0; cur_node != NULL; len++) {
        cur_node = cur_node->next;
    }

    return len;
}
