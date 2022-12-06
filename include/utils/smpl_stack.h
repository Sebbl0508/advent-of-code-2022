#pragma once

#include <stdint.h>
#include <stdlib.h>

/*
 * SimpleStack
 * A simple stack implementation
 * for some AoC tasks
 *
 * This is probably not the best solution
 * for a variable-sized-items stack,
 * but whatever ¯\_(ツ)_/¯
 *
 * This stack will never automatically shrink.
 * To do so, use the 'sstack_shrink_to_fit' function
 */

typedef struct SStack {
    /// The size of one item in the stack
    size_t item_size;

    /// The capacity in bytes
    size_t capacity;

    /// Number of items in the stack.
    /// This is important to check if the stack is completely empty
    size_t len;

    /// Pointer to the memory of the stack
    uint8_t* start;
} SStack;


/// @brief Create a stack
/// @param item_size Size of the to-be-inserted items in bytes: 'sizeof(type)'
/// @param capacity Pre-defined capacity of the stack in number of elements, can be 0
/// @return The stack object
SStack sstack_create(size_t item_size, size_t capacity);

/// @brief Push a item to the stack
/// @param stack The Stack object returned from 'sstack_create'
/// @param item A pointer to a item, which will be copied to the stack.
/// The item has to be exactly the item size specified when creating the stack
void sstack_push(SStack* stack, void* item);

/// @brief Shrinks the allocated capacity to the actual length
/// @param stack The Stack object returned from 'sstack_create'
void sstack_shrink_to_fit(SStack* stack);

/// @brief Pops the item at the top of the stack
/// @param stack The Stack object returned from 'sstack_create'
/// @return A pointer to the popped item.
/// @warning The memory of the returned pointer will be overwritten by the next call to 'sstack_pop'
void* sstack_pop(SStack* stack);


/// @brief Destroys the whole stack, freeing all memory
/// @param stack The Stack object returned from 'sstack_create'
void sstack_destroy(SStack* stack);
