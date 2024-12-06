// INCLUDES ----------------------------------------------------------------------------------------------------------------

#include <stddef.h> // "NULL"
#include <stdlib.h> // "malloc"
#include <stdio.h> // "printf"
#include <string.h> // "strcmp"

// MACROS ------------------------------------------------------------------------------------------------------------------

#define LL_APPEND(my_linked_list, string) list_append((struct list_node**)&my_linked_list, (struct list_node*)string_item_new(string));

// To define a multiple rows code use "\".
#define PRINT_LIST(first_item) while (first_item)                                                \
                               {                                                                 \
                                    printf("%s\n", first_item->string);                          \
                                    first_item = (struct string_item*)first_item->node.next;     \
                               }                                                                 \

#define LL_REMOVE_ITEM(my_linked_list, item, remove_item) remove_string_item((struct list_node**)&my_linked_list, item, remove_item);
#define LL_IS_NULL(my_linked_list) if(!my_linked_list) return NULL;
#define LL_IS_NULL_INT(my_linked_list) if(!my_linked_list) return -1;
#define LL_EQUAL_ITEMS(item_list, item) strcmp(item_list, item) == 0
#define PRINT_SEPARATORS(string) printf("--------------------------------------\n%s\n", string);

// STRUCTS -----------------------------------------------------------------------------------------------------------------

struct list_node 
{
    struct list_node* next;
};

struct string_item // Linked List of strings
{
    struct list_node node;
    const char* string;
};

// DECLARATIONS ------------------------------------------------------------------------------------------------------------

struct list_node* list_get_tail(struct list_node** head);
struct list_node* list_append(struct list_node** head, struct list_node* item);
struct list_node* list_pop(struct list_node** head);
struct string_item* string_item_new(const char* string);
struct string_item* remove_string_item(struct list_node** head, const char* item_to_remove, const int remove_item);
struct list_node* list_insert_to_head(struct list_node** head, struct list_node* item);
int list_reversing(struct list_node** head);
long long get_length(struct list_node** head);
struct string_item* get_item(struct list_node** head, const char* string);