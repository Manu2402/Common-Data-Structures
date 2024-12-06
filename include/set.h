#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <linked_list.h>

typedef struct hash_map_items
{
    struct string_item* data;
} hash_map_items_t;

typedef struct hash_map_table
{
    hash_map_items_t* items;
    size_t slots;
} hash_map_table_t;

size_t djb33x_hash(const char* key, const size_t keylen);
void print_key(const char* key);
void hash_map_init(hash_map_table_t* hash_map);
int hash_map_setup(hash_map_table_t* hash_map, const size_t number_of_slots);
int hash_map_add(hash_map_table_t* hash_map, const char* item);
struct string_item* hash_map_remove(hash_map_table_t* hash_map, const char* item_to_remove);
int hash_map_contains(hash_map_table_t* hash_map, const char* item);
void hash_map_iter_keys(hash_map_table_t* hash_map, void (*callback)(const char* key));