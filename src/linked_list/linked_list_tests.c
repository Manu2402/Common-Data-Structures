#include <linked_list.h>

// TESTS -------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    struct string_item* my_linked_list = NULL;

    // Linked List Items
    LL_APPEND(my_linked_list, "Hello World");
    LL_APPEND(my_linked_list, "Test001");
    LL_APPEND(my_linked_list, "Test002");
    LL_APPEND(my_linked_list, "Last Item of the Linked List");

    struct string_item* string_item = my_linked_list;  
    PRINT_SEPARATORS("Print List:") // ✔
    PRINT_LIST(string_item)

    PRINT_SEPARATORS("Print List (Remove):") // ✔
    LL_REMOVE_ITEM(my_linked_list, "Test001", 1); 
    string_item = my_linked_list;
    PRINT_LIST(string_item)

    PRINT_SEPARATORS("Print Reversed List:") // ✔
    string_item = my_linked_list;
    if(list_reversing((struct list_node**)&string_item)) return -1;
    PRINT_LIST(string_item)

    // Last minute tests.
    long long my_linked_list_length = get_length((struct list_node**)&my_linked_list);
    if(my_linked_list_length == -1) return -1;
    printf("\nPrint List Length: %lld\n", my_linked_list_length); // ✔

    string_item = my_linked_list;
    struct string_item* got_item = get_item(string_item, "Hello World");
    if(!got_item) return -1;
    printf("Got item: %s", got_item->string);

    return 0;
}