#include "utils/smpl_stack.h"

#include <string.h>

SStack sstack_create(size_t item_size, size_t capacity) {
    SStack stack = {0};
    stack.top = 0;
    stack.item_size = item_size;
    stack.capacity = capacity;
    stack.len = 0;
    stack.start = NULL;

    return stack;
}

void sstack_push(SStack* stack, void* item) {
    size_t new_size_needed = (stack->len + 1) * stack->item_size;

    // If the capacity required is larger than the current one, allocate memory for this item.
    // This is not very efficiant, but i don't want to allocate more memory than i have to
    if(new_size_needed > stack->capacity) {
        stack->start = realloc(stack->start, stack->capacity + stack->item_size);
        stack->capacity += stack->item_size;
    }

    // Calculate the pointer where this item will end up
    void* new_item_destination = stack->start + ((stack->len + 1) * stack->item_size);

    // Copy the item onto the stack
    memcpy(new_item_destination, item, stack->item_size);

    stack->len += 1;

    // If this is the first item on the stack,
    // the top-index will still be zero
    if(stack->len == 1)
        stack->top = 0;
    else
        stack->top += 1;
}

void* sstack_pop(SStack* stack) {
    static void* item_cache = NULL;
    if(item_cache == NULL)
        item_cache = calloc(1, stack->item_size);

    if(stack->len < 1)
        return NULL;

    // Copy the popped item to the item_cache, so we can resize the stack already
    void* item_ptr = stack->start + (stack->top * stack->item_size);
    memcpy(item_cache, item_ptr, stack->item_size);

    if(stack->len == 1) {

    }
}

void sstack_destroy(SStack* stack) {

}