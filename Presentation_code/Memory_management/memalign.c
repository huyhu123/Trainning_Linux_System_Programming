#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {
    int alignment = 32; // Align memory to a 32-byte boundary
    size_t size = 100; // Size of memory to allocate

    // Allocate aligned memory
    void* ptr = memalign(alignment, size);
    if (ptr == NULL) {
        printf("Failed to allocate memory\n");
        return 1;
    }

    // Use the allocated memory
    printf("Memory successfully allocated at address: %p\n", ptr);

    // Free the allocated memory
    free(ptr);

    return 0;
}