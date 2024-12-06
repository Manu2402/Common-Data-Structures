#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linked_list.h>

typedef enum type
{
    INT, STRING // ...
} type_t;

typedef struct item
{
    const char* key;
    void* value;
} item_t;

typedef struct list_item
{
    struct list_node node;
    item_t item;
} list_item_t;

typedef struct hash_map_items
{
    list_item_t* items; // Head.
} hash_map_items_t;

typedef struct hash_map_table
{
    hash_map_items_t* slots;
    size_t number_of_slots;
    type_t value_type;
} hash_map_table_t;

size_t djb33x_hash(const char* key, const size_t keylen);
void hash_map_init(hash_map_table_t* hash_map, const type_t type);
int hash_map_setup(hash_map_table_t* hash_map, const size_t new_number_of_slots);
int hash_map_add(hash_map_table_t* hash_map, const char* key, void* value);
int hash_map_contains(hash_map_table_t* hash_map, const char* key);
void hash_map_iter_keys(hash_map_table_t* hash_map, void (*callback)(const char* key));
void hash_map_iter_values(hash_map_table_t* hash_map, void (*callback)(void* value, const type_t type));
void print_key(const char* key);
void print_value(void* value, const type_t type);
void print_item(const item_t* item, const type_t type);
void print_hash_map(const hash_map_table_t* hash_map, void (*callback)(const item_t* item, const type_t type));

// Reworked functions.
list_item_t* list_item_new(const char* key, void* value);
list_item_t* hash_map_remove(hash_map_table_t* hash_map, const char* item_to_remove, const int remove_item);
list_item_t* get_list_item(hash_map_table_t* hash_map, const char* getter_key);