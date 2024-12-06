#include <dictionary.h>

int main(int argc, char** argv)
{
    const char* keys[] = { "uno", "due", "tre", "hello", "x", "y", "z", "xyz", "hello world", "tre", "ciao" };
    int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

    const size_t hash_map_size = 4;

    const size_t number_of_items = sizeof(keys) / sizeof(const char*);

    printf("Number of elements: %llu\nHashMap slots: %llu\n", number_of_items, hash_map_size);

    for (size_t i = 0; i < number_of_items; i++)
    {
        const size_t hash = djb33x_hash(keys[i], strlen(keys[i]));
        printf("Element %llu: \"%s\" hash: %llu (0x%016llx) slot: %llu\n", i, keys[i], hash, hash, hash % hash_map_size);
    }

    hash_map_table_t dict;
    hash_map_init(&dict, INT);

    if(hash_map_setup(&dict, 4))
    {
        printf("Error!\n");
        return -1;
    }

    for (size_t i = 0; i < number_of_items; i++)
    {
        if(hash_map_add(&dict, keys[i], &values[i]))
        {
            printf("Error adding %s to dictionary\n", keys[i]);
            return -1;
        }
    }

    for (size_t i = 0; i < number_of_items; i++)
    {
        keys[i] = NULL;
    }

    char stack_keys[] = { 'a', 'b', 'c', 'd', 0 };
    int value = 69;
    if(hash_map_add(&dict, stack_keys, &value))
    {
        printf("Error adding %s to dictionary\n", stack_keys);
        return -1;
    }

    if(!hash_map_contains(&dict, argv[1]))
    {
        printf("Found %s in dictionary!\n", argv[1]);
    }
    else
    {
        printf("%s Not found in dictionary!\n", argv[1]);
    }

    if(hash_map_setup(&dict, 8)) // Rehash
    {
        printf("Unable to Rehash!\n");
    }

    if(!hash_map_contains(&dict, argv[1]))
    {
        printf("Found %s in dictionary!\n", argv[1]);
    }
    else
    {
        printf("%s Not found in dictionary!\n", argv[1]);
    }

    hash_map_iter_keys(&dict, print_key);
    hash_map_iter_values(&dict, print_value);

    list_item_t* item_removed = hash_map_remove(&dict, "uno", 1);
    if(item_removed)
    {
        printf("Item \"%s\" found and removed!\n", item_removed->item.key);
    }
    else
    {
        printf("Item not found or dictionary was null!\n");
    }

    print_hash_map(&dict, print_item);

    return 0;
}