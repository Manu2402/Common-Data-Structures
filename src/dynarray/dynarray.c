#include <dynarray.h>

size_t malloc_counter = 0;
size_t malloc_total_allocations = 0;

void* aiv_realloc(void* data, const size_t new_size, const size_t current_size)
{
    if(new_size == 0)
    {
        malloc_total_allocations -= current_size; // As free().
    }
    else
    {
        malloc_counter++;
        malloc_total_allocations += new_size - malloc_total_allocations;
    }

    return realloc(data, new_size);
}

void* aiv_malloc(const size_t length)
{
    void* data = malloc(length);
    NULL_CHECK_PTR(data)
    malloc_counter++;
    malloc_total_allocations += length;
    return data;
}

void aiv_free(void* data, const size_t amount)
{
    malloc_total_allocations -= amount;
    free(data);
}

void set_data_capacity_dynarray(dynarray_t* dynarray, void* data, const size_t capacity)
{
    dynarray->data = data;
    dynarray->capacity = capacity;
}

int dynarray_set_capacity(dynarray_t* dynarray, const size_t new_capacity)
{
    if(new_capacity < dynarray->length)
    {
        return -1;
    }

    if(new_capacity == 0)
    {
        dynarray->data = NULL;
        dynarray->capacity = 0;
        return 0;
    }

    if(dynarray->length == 0)
    {
        dynarray->data = NULL; // Set this to NULL to make "realloc" behaviour as a "malloc".
    }

    void* new_data = aiv_realloc(dynarray->data, SIZEOF(new_capacity), SIZEOF(dynarray->capacity));
    NULL_CHECK_INT(new_data)
    
    set_data_capacity_dynarray(dynarray, new_data, new_capacity);
 
    return 0;
}

int dynarray_reserve(dynarray_t* dynarray, const size_t elements)
{
    return dynarray_set_capacity(dynarray, dynarray->capacity + elements);
}

int dynarray_remove(dynarray_t* dynarray, const size_t index)
{
    if(index >= dynarray->length)
    {
        return -1;
    }

    if(dynarray->length > 0 && index == dynarray->length - 1) 
    {
        dynarray->length--;
        return 0;
    }

    // "uint8_t" because the void* has no size. The minimum size to work with indexing is "char", but char sucks
    // and i prefer to use "uint8_t". 
    memmove(DYNARRAY_UINT8_DATA + SIZEOF(index), DYNARRAY_UINT8_DATA + SIZEOF((index - 1)), SIZEOF((dynarray->length - index - 1)));
    dynarray->length--;

    return 0;
}

int dynarray_append(dynarray_t* dynarray, const void* new_item)
{
    return dynarray_insert(dynarray, dynarray->length, new_item);
}

int dynarray_init(dynarray_t* dynarray, const size_t capacity)
{
    dynarray->length = 0;
    if(dynarray_set_capacity(dynarray, capacity))
    {
        return -1;
    }

    return 0;
}

int dynarray_empty(dynarray_t* dynarray)
{
    dynarray->length = 0;
    return 0;
}

int dynarray_insert(dynarray_t* dynarray, const size_t index, const void* new_item)
{
    NULL_CHECK_INT(new_item)

    if(dynarray->length + 1 > dynarray->capacity)
    {
        if(dynarray_reserve(dynarray, 1))
        {
            return -1;
        }
    }

    if(index < dynarray->length)
    {
        memmove(DYNARRAY_UINT8_DATA + SIZEOF(index + 1), DYNARRAY_UINT8_DATA + SIZEOF(index), SIZEOF(dynarray->length - index));
    }

    memcpy(DYNARRAY_UINT8_DATA + SIZEOF(dynarray->length), new_item, SIZEOF(1));
    dynarray->length++;

    return 0; 
}