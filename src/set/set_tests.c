#include <set.h>

int main(int argc, char** argv)
{
    const char* items[] = { "uno", "due", "tre", "hello", "x", "y", "z", "xyz", "hello world", "tre", "ciao" };
    //const char* items[] = { "uno", "due", "tre", "hello", "x", "y", "z", "xyz", "hello world", "test", "ciao" };

    const size_t hash_map_size = 4;

    const size_t number_of_items = sizeof(items) / sizeof(const char*);

    printf("Number of elements: %llu\nHashMap slots: %llu\n", number_of_items, hash_map_size);

    for (size_t i = 0; i < number_of_items; i++)
    {
        const size_t hash = djb33x_hash(items[i], strlen(items[i]));
        printf("Element %llu: \"%s\" hash: %llu (0x%016llx) slot: %llu\n", i, items[i], hash, hash, hash % hash_map_size);
    }

    hash_map_table_t set;
    hash_map_init(&set);

    if(hash_map_setup(&set, 4))
    {
        printf("Error!\n");
        return -1;
    }

    for (size_t i = 0; i < number_of_items; i++)
    {
        if(hash_map_add(&set, items[i]))
        {
            printf("Error adding %s to set\n", items[i]);
            return -1;
        }
    }

    // Test for integrity.
    for (size_t i = 0; i < number_of_items; i++)
    {
        items[i] = NULL;
    }

    char stack_data[] = { 'a', 'b', 'c', 'd', 0 };
    if(hash_map_add(&set, stack_data))
    {
        printf("Error adding %s to set\n", stack_data);
        return -1;
    }

    if(!hash_map_contains(&set, argv[1]))
    {
        printf("Found %s in set!\n", argv[1]);
    }
    else
    {
        printf("%s Not found in set!\n", argv[1]);
    }

    if(hash_map_setup(&set, 8)) // Rehash
    {
        printf("Unable to Rehash!\n");
    }

    if(!hash_map_contains(&set, argv[1]))
    {
        printf("Found %s in set!\n", argv[1]);
    }
    else
    {
        printf("%s Not found in set!\n", argv[1]);
    }

    hash_map_iter_keys(&set, print_key);

    struct string_item* item_removed = hash_map_remove(&set, "due");
    if(item_removed)
    {
        printf("Item \"%s\" found and removed!\n", item_removed->string);
    }
    else
    {
        printf("Item not found or set was null!");
    }

    hash_map_iter_keys(&set, print_key);

    return 0;
}