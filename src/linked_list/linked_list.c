#include <linked_list.h>

#ifdef _WIN32
    #define LL_API __declspec(dllexport)
#else
    #define LL_API
#endif

// FUNCTIONS ---------------------------------------------------------------------------------------------------------------
 
LL_API struct list_node* list_get_tail(struct list_node** head)
{
    struct list_node* current_node = *head;
    struct list_node* last_node = NULL;

    while (current_node) // while (current_node != NULL (aka != 0))
    {
        last_node = current_node;
        current_node = current_node->next;
    }

    return last_node;
}

LL_API struct list_node* list_append(struct list_node** head, struct list_node* item)
{
    struct list_node* tail = list_get_tail(head); // Get the tail.
    if (!tail) // Check if the list is empty.
    {
        *head = item;
    }
    else
    {
        tail->next = item;
    }

    item->next = NULL;
    return item;
}

LL_API struct list_node* list_pop(struct list_node** head)
{
    struct list_node* current_head = *head;
    LL_IS_NULL(current_head)

    *head = (*head)->next;
    current_head->next = NULL; // Remove references

    return current_head;
}

LL_API struct string_item* string_item_new(const char* string)
{
    struct string_item* item = malloc(sizeof(struct string_item));
    LL_IS_NULL(item)

    item->string = string;
    return item;
}

// EXERCISES ---------------------------------------------------------------------------------------------------------------

// Remove the first recurrency of the item in the list.
LL_API struct string_item* remove_string_item(struct list_node** head, const char* item_to_remove, const int remove_item)
{
    LL_IS_NULL(head)

    struct string_item* my_linked_list = (struct string_item*)(*head);
    struct list_node* prev_item = (struct list_node*)head;

    while(my_linked_list)
    {
        if(LL_EQUAL_ITEMS(my_linked_list->string, item_to_remove)) // String match
        {
            if(remove_item)
            {
                prev_item->next = my_linked_list->node.next;
                my_linked_list->node.next = NULL;
            }

            return my_linked_list;
        }

        prev_item = (struct list_node*)my_linked_list;
        my_linked_list = (struct string_item*)my_linked_list->node.next;
    }

    return NULL;
}

LL_API struct list_node* list_insert_to_head(struct list_node** head, struct list_node* item)
{
    struct string_item* current_item = (struct string_item*)item;
    struct list_node* new_item = (struct list_node*)string_item_new(current_item->string);

    if(*head == NULL)
    { 
        *head = new_item;
        new_item->next = NULL;
    }
    else
    {     
        struct list_node* old_first_item = (struct list_node*)(*head);
        *head = new_item;
        new_item->next = old_first_item;
    }

    return item;
}

LL_API int list_reversing(struct list_node** head)
{
    LL_IS_NULL_INT(head)

    struct list_node* first_item = *head;
    struct string_item* reversed_list_head = NULL;

    struct list_node* item_to_free = NULL;

    while(first_item)
    {
        list_insert_to_head((struct list_node**)&reversed_list_head, first_item);

        item_to_free = first_item;
        first_item = first_item->next;
        free(item_to_free); 
    }

    item_to_free = NULL;
    
    *head = (struct list_node*)reversed_list_head;

    return 0;
}

LL_API long long get_length(struct list_node** head)
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
        ++length;
        first_element = first_element->next;
    }

    return length;
}

LL_API struct string_item* get_item(struct list_node** head, const char* string)
{
    return LL_REMOVE_ITEM(head, string, 0); 
}