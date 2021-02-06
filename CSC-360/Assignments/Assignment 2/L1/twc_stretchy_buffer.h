#ifndef TWC_STRETCHY_BUFFER
#define TWC_STRETCHY_BUFFER

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// This changes how much the arrays capacity the arrays gain when reallocated
#define capacity_increment                                                     \
    5 

// Get the header of the array
#define array_header(a)                                                        \
    ((twc_array_header_t *)((char *)(a) - sizeof(twc_array_header_t)))

// Gets the size of the array
#define array_size(a) ((a) ? array_header(a)->size : 0)

// Gets the capacity of the array, returns zero if array is null
#define array_capacity(a) ((a) ? array_header(a)->capacity : 0)

// Gets whether or not the array is at capacity, if array is null returns true
#define array_full(a) (array_size(a) == array_capacity(a))

// Adds a item to a array, resizes the array if full
#define array_push(a, item)                                                    \
    array_full(a) ? a = array_grow(a, sizeof(*a)) : 0,                         \
                    a[array_header(a)->size++] = item

// Frees the array, NOTE: must be freed this way as a does not point to the
// begining
#define array_free(a) ((a) ? free(array_header(a)), 0 : 0)

typedef struct twc_array_header_t {
    uint32_t capacity;
    uint32_t size;
} twc_array_header_t;

/**
 * Adds capacity to the struct
 */
static void *array_grow(void *array, uint32_t itemsize) {
    // Get the current capacity
    uint32_t current_capacity =
        array_capacity(array); // Note this will be zero if the array is null

    // Note this works because passing a 0 to realloc makes it work like malloc
    // The size of the buffer is the size of the header + the size of the item
    // we are storing times the new capacity we want
    twc_array_header_t *ptr =
        realloc(array ? array_header(array) : 0,
                sizeof(twc_array_header_t) +
                    itemsize * (current_capacity + capacity_increment));

    // ASSUMES ALLOCATION DOES NOT FAIL
    // Note: ptr points at the struct
    // Set the array capacity
    ptr->capacity = current_capacity + capacity_increment;

    if (!array) { // if the array was uninitialized set its size to 0
        ptr->size = 0;
    }

    // Return a pointer to the first element in the array after the header
    // Note because this is casted as a pointer to twc_array_header_t adding 1
    // to it actually ads 8 Another way of doing this would be to cast it to a
    // char here and add sizeof(twc_array_header_t) eg. return ((char*)ptr) +
    // sizeof(twc_array_header_t)
    return ptr + 1;
}

#endif