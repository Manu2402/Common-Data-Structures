#include <doubly_linked_list.h>

// FUNCTIONS ---------------------------------------------------------------------------------------------------------------
 
struct list_node* list_get_tail(struct list_node** head)
{
    struct list_node* current_node = *head;
    struct list_node* last_node = NULL;

    while (current_node)
    {
        last_node = current_node;
        current_node = current_node->next;
    }

    return last_node;
}

struct list_node* list_append(struct list_node** head, struct list_node* item)
{
    struct list_node* tail = list_get_tail(head);
    if (!tail)
    {
        *head = item;
    }
    else
    {
        tail->next = item;
    }

    item->prev = tail;
    item->next = NULL;
    return item;
}

struct list_node* list_pop(struct list_node** head)
{
    struct list_node* current_head = *head;
    DLL_IS_NULL(current_head)

    *head = (*head)->next;
    (*head)->prev = NULL;

    current_head->next = NULL; // Remove references.

    return current_head;
}

struct string_item* string_item_new(const char* string)
{
    struct string_item* item = malloc(sizeof(struct string_item));
    DLL_IS_NULL(item)

    item->string = string;
    return item;
}

// EXERCISES ---------------------------------------------------------------------------------------------------------------

// Remove the first recurrency of the item in the list.
struct string_item* remove_string_item(struct list_node** head, const char* item_to_remove)
{
    DLL_IS_NULL(head)

    struct string_item* my_doubly_linked_list = (struct string_item*)(*head);

    while(my_doubly_linked_list)
    { 
        if(DLL_EQUAL_ITEMS(my_doubly_linked_list->string, item_to_remove)) // String match
        {
            if(my_doubly_linked_list->node.prev)
            {
                my_doubly_linked_list->node.prev->next = my_doubly_linked_list->node.next;
            }
            else
            {
                (*head) = (struct list_node*)my_doubly_linked_list->node.next;   
            }

            if(my_doubly_linked_list->node.next)
            {
                my_doubly_linked_list->node.next->prev = my_doubly_linked_list->node.prev;
            }

            my_doubly_linked_list->node.prev = NULL;
            my_doubly_linked_list->node.next = NULL;

            return my_doubly_linked_list;
        }

        my_doubly_linked_list = (struct string_item*)my_doubly_linked_list->node.next;
    }

    return NULL;
}

// Insert after the first recurrency of the item in the list.
int insert_after(struct list_node** head, struct string_item* item_to_insert, const char* item_reference)
{
    DLL_IS_NULL_INT(head)
    
    if(!item_to_insert)
    {
        return -1;
    }

    struct string_item* my_doubly_linked_list = (struct string_item*)(*head);

    while(my_doubly_linked_list)
    {
        if(DLL_EQUAL_ITEMS(my_doubly_linked_list->string, item_reference)) // String match
        {  
            item_to_insert->node.prev = (struct list_node*)my_doubly_linked_list;
            item_to_insert->node.next = my_doubly_linked_list->node.next;

            if(my_doubly_linked_list->node.next)
            {
                my_doubly_linked_list->node.next->prev = (struct list_node*)item_to_insert;
            }

            my_doubly_linked_list->node.next = (struct list_node*)item_to_insert;

            return 0;
        }

        my_doubly_linked_list = (struct string_item*)my_doubly_linked_list->node.next;
    }

    return 0;
}

// Insert before the first recurrency of the item in the list.
int insert_before(struct list_node** head, struct string_item* item_to_insert, const char* item_reference)
{
    DLL_IS_NULL_INT(head)
    
    if(!item_to_insert)
    {
        return -1;
    }

    struct string_item* my_doubly_linked_list = (struct string_item*)(*head);

    while(my_doubly_linked_list)
    {
        if(DLL_EQUAL_ITEMS(my_doubly_linked_list->string, item_reference)) // String match
        {  
            item_to_insert->node.prev = my_doubly_linked_list->node.prev;
            item_to_insert->node.next = (struct list_node*)my_doubly_linked_list;

            if(my_doubly_linked_list->node.prev)
            {
                my_doubly_linked_list->node.prev->next = (struct list_node*)item_to_insert;
            }
            else
            {
                (*head) = (struct list_node*)item_to_insert;
            }

            my_doubly_linked_list->node.prev = (struct list_node*)item_to_insert;

            return 0;
        }

        my_doubly_linked_list = (struct string_item*)my_doubly_linked_list->node.next;
    }

    return 0;
}

long long get_length(struct list_node** head)
{
    if(!head)
    {
        return -1;
    }

    if(*head == NULL)
    {
        return 0;
    }

    struct list_node* first_element = *head;
    size_t length = 0;

    while(first_element)
    {
        length++;
        first_element = first_element->next;
    }

    return length;
}

int shuffle(struct list_node** head)
{
    DLL_IS_NULL_INT(head)

    srand((unsigned)time(NULL)); // Setting "seed" for pseudo-random number generation.

    size_t list_length = get_length(head);
    size_t temp_length = list_length;
    struct string_item* shuffled_list_head = NULL;

    struct list_node* current_item = NULL;
    struct string_item* actual_item = NULL;
    struct string_item* removed_item = NULL;

    for(size_t i = 0; i < list_length; i++)
    {
        size_t random = rand() % temp_length;

        current_item = *head;
        for(size_t i = 0; i < random; i++)
        {
            current_item = current_item->next;
        }

        // May be more worth to remove an item based on the index, but for shuffle doesn't care too.
        actual_item = (struct string_item*)current_item;  
        removed_item = remove_string_item(head, actual_item->string);
        DLL_APPEND(shuffled_list_head, removed_item->string);

        free(removed_item);
  
        temp_length--;
    }

    *head = (struct list_node*)shuffled_list_head;

    return 0;
}