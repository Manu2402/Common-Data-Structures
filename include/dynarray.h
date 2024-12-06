#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define SIZEOF(elements) dynarray->size_of_element * elements
#define NULL_CHECK_INT(ptr) if(!ptr) return -1;
#define NULL_CHECK_PTR(ptr) if(!ptr) return NULL;
#define DYNARRAY_UINT8_DATA (uint8_t*)dynarray->data

extern size_t malloc_counter;
extern size_t malloc_total_allocations;

typedef struct bullet // Useless
{
    float speed;
    float x;
    float y;
    float z;
    float dx;
    float dy;
    float dz;
    float lifetime;
} bullet_t;

typedef struct str_example // Useless
{
    int first;
    float second;
    char third;
    char fourth;
    short fifth;
    size_t sixth;
} str_example_t;

typedef struct dynarray
{
    void* data; // Generic type.
    size_t length; 
    size_t capacity;
    size_t size_of_element; // It's already a sizeof (number of bytes).
} dynarray_t;

void* aiv_realloc(void* data, const size_t new_size, const size_t current_size);
void* aiv_malloc(const size_t length);
void aiv_free(void* data, const size_t amount);
void set_data_capacity_dynarray(dynarray_t* dynarray, void* data, const size_t capacity);
int dynarray_set_capacity(dynarray_t* dynarray, const size_t new_capacity);
int dynarray_reserve(dynarray_t* dynarray, const size_t elements);
int dynarray_remove(dynarray_t* dynarray, const size_t index);
int dynarray_insert(dynarray_t* dynarray, const size_t index, const void* new_item);
int dynarray_append(dynarray_t* dynarray, const void* new_item);
int dynarray_init(dynarray_t* dynarray, const size_t capacity);
int dynarray_empty(dynarray_t* dynarray);