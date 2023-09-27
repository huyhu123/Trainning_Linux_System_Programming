#include "dynamic_memory_allocation.h"

int main()
{
    // Test malloc
    char *ptr1 = NULL;
    ptr1 = (char *)my_malloc(10);
    strcpy(ptr1, "Hello");
    printf("Test malloc: %s\n", ptr1);
    my_free(ptr1);

    // Test realloc
    char *ptr2 = NULL;
    ptr2 = (char *)my_malloc(5);
    strcpy(ptr2, "Hello");
    ptr2 = (char *)my_realloc(ptr2, 10);
    strcat(ptr2, " World");
    printf("Test realloc: %s\n", ptr2);
    my_free(ptr2);

    return 0;
}
