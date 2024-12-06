#include <dynarray.h>

// TESTS ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    dynarray_t dynarray;
    dynarray.size_of_element = sizeof(str_example_t); // sizeof element.
    // dynarray.size_of_element = sizeof(bullet_t);
    // dynarray.size_of_element = sizeof(uint16_t);

    if(dynarray_init(&dynarray, 10))
    {
        printf("Unable to initialize dynarray!\n");
        return -1;
    }

    str_example_t item0;
    if(dynarray_append(&dynarray, &item0))
    {
        printf("Unable to append element at slot %llu!\n", dynarray.length); 
        return -1;
    }

    dynarray_reserve(&dynarray, 1024);
    for (size_t i = 0; i < 1024; i++)
    {
        if(dynarray_append(&dynarray, &item0))
        {
            printf("Unable to append element at slot %llu!\n", dynarray.length); 
            return -1;
        }
    }
    
    printf("Appended %llu elements with capacity %llu and %llu calls to malloc\n", dynarray.length, dynarray.capacity, malloc_counter);
    printf("sizeof(%s) = %llu *** sizeof(%s) * %llu = %llu\n", "str_example_t", dynarray.size_of_element, "str_example_t", dynarray.length, dynarray.size_of_element * dynarray.length);
    printf("malloc total allocations: %llu\n", malloc_total_allocations);
    printf("\n");

    dynarray_remove(&dynarray, 0);

    printf("Appended %llu elements with capacity %llu and %llu calls to malloc\n", dynarray.length, dynarray.capacity, malloc_counter);
    printf("sizeof(%s) = %llu *** sizeof(%s) * %llu = %llu\n", "str_example_t", dynarray.size_of_element, "str_example_t", dynarray.length, dynarray.size_of_element * dynarray.length);
    printf("malloc total allocations: %llu\n", malloc_total_allocations);
    printf("\n");

    dynarray_remove(&dynarray, 1023);

    printf("Appended %llu elements with capacity %llu and %llu calls to malloc\n", dynarray.length, dynarray.capacity, malloc_counter);
    printf("sizeof(%s) = %llu *** sizeof(%s) * %llu = %llu\n", "str_example_t", dynarray.size_of_element, "str_example_t", dynarray.length, dynarray.size_of_element * dynarray.length);
    printf("malloc total allocations: %llu\n", malloc_total_allocations);
    printf("\n");

    dynarray_remove(&dynarray, 100);

    printf("Appended %llu elements with capacity %llu and %llu calls to malloc\n", dynarray.length, dynarray.capacity, malloc_counter);
    printf("sizeof(%s) = %llu *** sizeof(%s) * %llu = %llu\n", "str_example_t", dynarray.size_of_element, "str_example_t", dynarray.length, dynarray.size_of_element * dynarray.length);
    printf("malloc total allocations: %llu\n", malloc_total_allocations);
 
    return 0;
}