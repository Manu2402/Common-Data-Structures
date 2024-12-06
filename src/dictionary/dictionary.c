#include <dictionary.h>

static size_t djb33x_hash(const char* key, const size_t keylen)
{
    size_t hash = 5381;

    for (size_t i = 0; i < keylen; i++)
    {
        hash = ((hash << 5) + hash) ^ key[i];
    }

    return hash;
}

void hash_map_init(hash_map_table_t* hash_map, const type_t type)
{
    hash_map->slots = NULL;
    hash_map->number_of_slots = 0;
    hash_map->value_type = type;
}

int hash_map_setup(hash_map_table_t* hash_map, const size_t new_number_of_slots)
{
    if(hash_map->number_of_slots == 0)
    {
        hash_map->number_of_slots = new_number_of_slots;
        hash_map->slots = malloc(sizeof(hash_map_items_t) * hash_map->number_of_slots);

        if(!hash_map->slots) return -1;

        memset(hash_map->slots, 0, sizeof(hash_map_items_t) * hash_map->number_of_slots);
    }
    else
    {
        printf("Rehashing with %llu\n", new_number_of_slots);
        hash_map_items_t* new_slots = malloc(sizeof(hash_map_items_t) * new_number_of_slots);
        memset(new_slots, 0, sizeof(hash_map_items_t) * new_number_of_slots);

        for (size_t slot_index = 0; slot_index < hash_map->number_of_slots; slot_index++)
        {
            hash_map_items_t* slot_items = &hash_map->slots[slot_index];
            list_item_t* first_item = slot_items->items;

            while(first_item)
            {
                const char* key = first_item->item.key;
                void* value = first_item->item.value;

                printf("Rehashing %s ...\n", key);

                const size_t hash = djb33x_hash(key, strlen(key));
                const size_t new_slot = hash % new_number_of_slots;

                list_append((struct list_node**)&new_slots[new_slot].items, (struct list_node*)list_item_new(key, value));
                first_item = (list_item_t*)first_item->node.next;
            }
        }
        
        free(hash_map->slots);
        hash_map->slots = new_slots;
        hash_map->number_of_slots = new_number_of_slots;
    }

    return 0;
}

list_item_t* list_item_new(const char* key, void* value)
{
    list_item_t* list_item = malloc(sizeof(list_item_t));
    LL_IS_NULL(list_item)

    list_item->item.key = key;
    list_item->item.value = value;

    return list_item;
}

int hash_map_add(hash_map_table_t* hash_map, const char* key, void* value)
{
    const size_t hash = djb33x_hash(key, strlen(key));
    const size_t slot = hash % hash_map->number_of_slots;

    switch (hash_map->value_type)
    {
        case INT:
            printf("Adding %s - %d (hash: %016llx) in slot %llu\n", key, *(int*)value, hash, slot);
            break;
        case STRING:
            printf("Adding %s - %s (hash: %016llx) in slot %llu\n", key, (const char*)value, hash, slot);
            break;
        default:
            break;
    }

    hash_map_items_t* slot_items = &hash_map->slots[slot];

    if(!hash_map_contains(hash_map, key))
    {
        printf("Key %s already sets into the linked list!\n", key);
        return 0;
    }

    size_t slot_length = get_length((struct list_node**)&slot_items->items);

    if(slot_length > 1)
    {
        if(hash_map_setup(hash_map, hash_map->number_of_slots * 2))
        {
            printf("Rehashing failed!\n");
            return -1;
        }

        const size_t new_slot = hash % hash_map->number_of_slots;
        slot_items = &hash_map->slots[new_slot];
    }

    list_append((struct list_node**)&slot_items->items, (struct list_node*)list_item_new(key, value));

    return 0;
}

int hash_map_contains(hash_map_table_t* hash_map, const char* key)
{
    return get_list_item(hash_map, key) ? 0 : -1;
}

void hash_map_iter_keys(hash_map_table_t* hash_map, void (*callback)(const char* key))  
{
    printf("PRINTING KEYS...\n");
    for (size_t slot_index = 0; slot_index < hash_map->number_of_slots; slot_index++)
    {
        hash_map_items_t* slot_items = &hash_map->slots[slot_index];
        struct list_item* first_item = slot_items->items;

        while(first_item)
        {
            callback(first_item->item.key);
            first_item = (struct list_item*)first_item->node.next;
        }
    }
}

void hash_map_iter_values(hash_map_table_t* hash_map, void (*callback)(void* value, const type_t type))
{
    printf("PRINTING VALUES...\n");
    for (size_t slot_index = 0; slot_index < hash_map->number_of_slots; slot_index++)
    {
        hash_map_items_t* slot_items = &hash_map->slots[slot_index];
        struct list_item* first_item = slot_items->items;

        while(first_item)
        {
            callback(first_item->item.value, hash_map->value_type);
            first_item = (struct list_item*)first_item->node.next;
        }
    }
}

void print_key(const char* key)
{
    printf("Hello i am print_key() for %s\n", key);
}

void print_value(void* value, const type_t type)
{
    switch (type)
    {
        case INT:
            printf("Hello i am print_value() for %d\n", *(int*)value);
            break;
        case STRING:
            printf("Hello i am print_value() for %s\n", (const char*)value);
            break;
        default:
            break;
    }
}

list_item_t* hash_map_remove(hash_map_table_t* hash_map, const char* item_to_remove, const int remove_item)
{
    if(!item_to_remove) return NULL;

    const size_t hash = djb33x_hash(item_to_remove, strlen(item_to_remove));
    const size_t slot = hash % hash_map->number_of_slots;

    hash_map_items_t* slot_items = &hash_map->slots[slot];
    list_item_t* first_item = slot_items->items;
    struct list_node* prev_item = (struct list_node*)&first_item;

    while(first_item)
    {
        if(LL_EQUAL_ITEMS(first_item->item.key, item_to_remove)) // String match
        {      
            if(remove_item)
            {
                prev_item->next = first_item->node.next;
                first_item->node.next = NULL;
            }

            return first_item;
        }

        prev_item = (struct list_node*)first_item;
        first_item = (list_item_t*)first_item->node.next;
    }

    return NULL;
}

list_item_t* get_list_item(hash_map_table_t* hash_map, const char* getter_key)
{
    if(!getter_key) return NULL;
    return hash_map_remove(hash_map, getter_key, 0);
}

void print_item(const item_t* item, const type_t type)
{
    switch (type)
    {
        case INT:
            printf("Hello i am print_item() for key: %s with value: %d\n", item->key, *(int*)item->value);
            break;
        case STRING:
            printf("Hello i am print_item() for key: %s with value: %s\n", item->key, (const char*)item->value);
            break;
        default:
            break;
    }
}

void print_hash_map(const hash_map_table_t* hash_map, void (*callback)(const item_t* item, const type_t type))
{
    for (size_t slot_index = 0; slot_index < hash_map->number_of_slots; slot_index++)
    {
        hash_map_items_t* slot_items = &hash_map->slots[slot_index];
        list_item_t* first_item = slot_items->items;

        while(first_item)
        {
            callback(&first_item->item, hash_map->value_type);
            first_item = (list_item_t*)first_item->node.next;
        }
    }
}