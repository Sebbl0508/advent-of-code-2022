#include "utils/smpl_stack.h"

#include <string.h>

SStack sstack_create(size_t item_size, size_t capacity) {
    SStack stack = {0};
    stack.item_size = item_size;
    stack.capacity = capacity * item_size;
    stack.len = 0;
    stack.start = malloc(stack.capacity);

    return stack;
}

void sstack_push(SStack* stack, void* item) {
    size_t new_size_needed = (stack->len + 1) * stack->item_size;

    // If the capacity required is larger than the current capacity
    // increase it by the half the current item-capacity
    //
    // Example: items and capacity are both at 6,
    //          if there is an item pushed the space is expanded
    //          to allow 2 more items after the push
    if(new_size_needed > stack->capacity) {
        size_t additional_space = (stack->len / 2) * stack->item_size;
        if(additional_space < stack->item_size)
            additional_space = stack->item_size;

        stack->start = realloc(stack->start, stack->capacity + additional_space);
        stack->capacity += additional_space;
    }

    // Calculate the slot where the new item will be copied to
    void* new_item_dest = stack->start + (stack->len * stack->item_size);

    // Copy the item into the slot and increase the stack length
    memcpy(new_item_dest, item, stack->item_size);
    stack->len += 1;
}

void* sstack_pop(SStack* stack) {
    if(stack->len == 0)
        return NULL;

    // Initialize a item cache,
    // so we can manipulate the stack
    // without the returned pointer becoming invalid
    static void* item_cache = NULL;
    if(item_cache == NULL)
        item_cache = malloc(stack->item_size);

    // Calculate where our last item lays & copy it to the cache
    void* item_ptr = stack->start + ((stack->len - 1) * stack->item_size);
    memcpy(item_cache, item_ptr, stack->item_size);

    stack->len -= 1;

    // Return the pointer to the cache
    return item_cache;
}

void sstack_destroy(SStack* stack) {
    free(stack->start);
    memset(stack, 0, sizeof(SStack));
}

void sstack_shrink_to_fit(SStack* stack) {
    size_t actually_needed_capacity = stack->len * stack->item_size;
    stack->start = realloc(stack->start, actually_needed_capacity);
    stack->capacity = actually_needed_capacity;
}
