#include <doubly_linked_list.h>

// TESTS -------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    struct string_item* my_doubly_linked_list = NULL;

    // Linked List Items
    DLL_APPEND(my_doubly_linked_list, "Hello World");
    DLL_APPEND(my_doubly_linked_list, "Test001");
    DLL_APPEND(my_doubly_linked_list, "Test001");
    DLL_APPEND(my_doubly_linked_list, "Last Item of the Doubly Linked List");

    PRINT_SEPARATORS("Print List:") // ✔
    struct string_item* string_item = my_doubly_linked_list;  
    PRINT_LIST(string_item)

    PRINT_SEPARATORS("Print List (Remove):") // ✔
    DLL_REMOVE_ITEM(my_doubly_linked_list, "Last Item of the Doubly Linked List")
    string_item = my_doubly_linked_list;  
    PRINT_LIST(string_item)

    PRINT_SEPARATORS("Print List (Insert After):") // ✔
    DLL_INSERT_AFTER(my_doubly_linked_list, "After Last Item of the Doubly Linked List", "Last Item of the Doubly Linked List")
    string_item = my_doubly_linked_list;
    PRINT_LIST(string_item)

    PRINT_SEPARATORS("Print List (Insert Before):") // ✔
    DLL_INSERT_BEFORE(my_doubly_linked_list, "Before Hello World", "Hello World")
    string_item = my_doubly_linked_list;
    PRINT_LIST(string_item)

    PRINT_SEPARATORS("Print Shuffled List:") // ✔
    if(shuffle((struct list_node**)&my_doubly_linked_list)) return -1;
    string_item = my_doubly_linked_list;
    PRINT_LIST(string_item)

    return 0;
}