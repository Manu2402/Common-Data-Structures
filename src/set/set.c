#include <set.h>

static size_t djb33x_hash(const char* key, const size_t keylen)
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

void hash_map_init(hash_map_table_t* hash_map)
{
    hash_map->items = NULL;
    hash_map->slots = 0;
}

int hash_map_setup(hash_map_table_t* hash_map, const size_t number_of_slots)
{
    if(hash_map->slots == 0)
    {
        hash_map->slots = number_of_slots;
        hash_map->items = malloc(sizeof(hash_map_items_t) * hash_map->slots);

        if(!hash_map->items) return -1;

        memset(hash_map->items, 0, sizeof(hash_map_items_t) * hash_map->slots);
    }
    else
    {
        printf("Rehashing with %llu\n", number_of_slots);
        hash_map_items_t* new_items = malloc(sizeof(hash_map_items_t) * number_of_slots);
        memset(new_items, 0, sizeof(hash_map_items_t) * number_of_slots);

        for (size_t slot_index = 0; slot_index < hash_map->slots; slot_index++)
        {
            hash_map_items_t* slot_items = &hash_map->items[slot_index];

            struct string_item* first_item = slot_items->data;

            while(first_item)
            {
                const char* key = first_item->string;
                printf("Rehashing %s ...\n", key);
                const size_t hash = djb33x_hash(key, strlen(key));
                const size_t new_slot = hash % number_of_slots;

                LL_APPEND(new_items[new_slot].data, key);
                first_item = (struct string_item*)first_item->node.next;
            }
        }
        
        free(hash_map->items);
        hash_map->items = new_items;
        hash_map->slots = number_of_slots;
    }

    return 0;
}

int hash_map_add(hash_map_table_t* hash_map, const char* item)
{
    const size_t hash = djb33x_hash(item, strlen(item));
    const size_t slot = hash % hash_map->slots;
    printf("Adding %s (hash: %016llx) in slot %llu\n", item, hash, slot);

    hash_map_items_t* slot_items = &hash_map->items[slot];

    if(!hash_map_contains(hash_map, item))
    {
        printf("Item %s already sets into the linked list!\n", item);
        return 0;
    }

    size_t slot_length = get_length((struct list_node**)slot_items);

    if(slot_length > 1)
    {
        if(hash_map_setup(hash_map, hash_map->slots * 2))
        {
            printf("Rehashing failed!\n");
            return -1;
        }

        const size_t new_slot = hash % hash_map->slots;
        slot_items = &hash_map->items[new_slot];
    }

    LL_APPEND(slot_items->data, item); // With "malloc" the keys are handled directly by the hash_map self.

    return 0;
}

int hash_map_contains(hash_map_table_t* hash_map, const char* item)
{
    if(!item) return -1;

    const size_t hash = djb33x_hash(item, strlen(item));
    const size_t slot = hash % hash_map->slots;
    printf("Checking %s (hash: %016llx) in slot %llu\n", item, hash, slot);

    hash_map_items_t* slot_items = &hash_map->items[slot];

    struct string_item* got_item = get_item((struct list_node**)slot_items->data, item);
    if(!got_item) return -1;
    
    return 0;
}

void hash_map_iter_keys(hash_map_table_t* hash_map, void (*callback)(const char* key))  
{
    for (size_t slot_index = 0; slot_index < hash_map->slots; slot_index++)
    {
        hash_map_items_t* slot_items = &hash_map->items[slot_index];
        struct string_item* first_item = slot_items->data;

        while(first_item)
        {
            callback(first_item->string);
            first_item = (struct string_item*)first_item->node.next;
        }
    }
}

static void print_key(const char* key)
{
    printf("Hello i am print_key() for %s\n", key);
}

struct string_item* hash_map_remove(hash_map_table_t* hash_map, const char* item_to_remove)
{
    size_t hash = djb33x_hash(item_to_remove, strlen(item_to_remove));
    size_t slot = hash % hash_map->slots;

    hash_map_items_t* current_slot = &hash_map->items[slot];

    return LL_REMOVE_ITEM(current_slot->data, item_to_remove, 1)
}