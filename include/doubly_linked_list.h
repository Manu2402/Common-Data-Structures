// INCLUDES ----------------------------------------------------------------------------------------------------------------

#include <stddef.h> // "NULL"
#include <stdlib.h> // "malloc", "(s)rand"
#include <stdio.h> // "printf"
#include <string.h> // "strcmp"
#include <time.h> // "time"

// MACROS ------------------------------------------------------------------------------------------------------------------

#define DLL_APPEND(my_doubly_linked_list, string) list_append((struct list_node**)&my_doubly_linked_list, (struct list_node*)string_item_new(string));

#define PRINT_LIST(first_item) while (first_item)                                                \
                               {                                                                 \
                                    printf("%s\n", first_item->string);                          \
                                    first_item = (struct string_item*)first_item->node.next;     \
                               }                                                                 \

#define DLL_IS_NULL(my_doubly_linked_list) if(!my_doubly_linked_list) return NULL;
#define DLL_IS_NULL_INT(my_doubly_linked_list) if(!my_doubly_linked_list) return -1;
#define DLL_EQUAL_ITEMS(item_list, item) strcmp(item_list, item) == 0
#define DLL_REMOVE_ITEM(my_doubly_linked_list, item) remove_string_item((struct list_node**)&my_doubly_linked_list, item);
#define PRINT_SEPARATORS(string) printf("--------------------------------------\n%s\n", string);
#define DLL_INSERT_AFTER(my_doubly_linked_list, insert_after_item, item) insert_after((struct list_node**)&my_doubly_linked_list, string_item_new(insert_after_item), item);
#define DLL_INSERT_BEFORE(my_doubly_linked_list, insert_before_item, item) insert_before((struct list_node**)&my_doubly_linked_list, string_item_new(insert_before_item), item);

// STRUCTS -----------------------------------------------------------------------------------------------------------------

struct list_node
{
    struct list_node* prev;
    struct list_node* next;
};

struct string_item // Doubly Linked List of strings
{
    struct list_node node;
    const char* string;
};

struct list_node* list_get_tail(struct list_node** head);
struct list_node* list_append(struct list_node** head, struct list_node* item);
struct list_node* list_pop(struct list_node** head);
struct string_item* string_item_new(const char* string);
struct string_item* remove_string_item(struct list_node** head, const char* item_to_remove);
int insert_after(struct list_node** head, struct string_item* item_to_insert, const char* item_reference);
int insert_before(struct list_node** head, struct string_item* item_to_insert, const char* item_reference);
long long get_length(struct list_node** head);
int shuffle(struct list_node** head);