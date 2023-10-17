/*
Note: memory allocated using alloca() is automatically deallocated when the function's scope is exited, so you don't need to explicitly free it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <alloca.h>

int main() {
    int count = 5; // Number of integers to allocate

    // Allocate memory on the stack
    int* arr = alloca(count * sizeof(int));

    // Initialize the allocated memory
    for (int i = 0; i < count; i++) {
        arr[i] = i + 1;
    }

    // Print the values
    printf("Allocated array values:\n");
    for (int i = 0; i < count; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}