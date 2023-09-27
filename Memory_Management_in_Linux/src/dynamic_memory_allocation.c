#include "dynamic_memory_allocation.h"

static meta_block_t *head = NULL;

void *my_malloc(size_t size)
{
    meta_block_t *current = NULL;
    meta_block_t *prev = NULL;
    meta_block_t *new_block = NULL;
    size_t total_size = 0;
    void *block = NULL;

    if (size <= 0)
    {
        return NULL;
    }

    current = head;
    while (current != NULL)
    {
        if (current->is_free && current->size >= size)
        {
            if (current->size > size + sizeof(meta_block_t))
            {
                // Split the block if it's large enough
                new_block = (meta_block_t *)((char *)(current + 1) + size);
                new_block->size = current->size - size - sizeof(meta_block_t);
                new_block->is_free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }
            current->is_free = false;
            return (void *)(current + 1);
        }
        prev = current;
        current = current->next;
    }

    // No suitable block found, allocate a new block using sbrk()
    total_size = sizeof(meta_block_t) + size;
    block = sbrk(total_size);
    if (block == (void *)-1)
    {
        return NULL; // Allocation failed
    }

    new_block = (meta_block_t *)block;
    new_block->size = size;
    new_block->is_free = false;
    new_block->next = NULL;

    if (prev != NULL)
    {
        prev->next = new_block;
    }
    else
    {
        head = new_block;
    }

    return (void *)(new_block + 1);
}

void my_free(void *ptr)
{
    meta_block_t *block = NULL;
    meta_block_t *current = NULL;

    if (ptr == NULL)
    {
        return;
    }

    block = (meta_block_t *)ptr - 1;
    block->is_free = true;

    // Merge consecutive free blocks
    current = head;
    while (current != NULL && current->next != NULL)
    {
        if (current->is_free && current->next->is_free)
        {
            current->size += sizeof(meta_block_t) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void *my_realloc(void *ptr, size_t size)
{
    meta_block_t *block = NULL;
    meta_block_t *new_block = NULL;
    void *new_ptr = NULL;
    size_t copy_size = 0;

    if (ptr == NULL)
    {
        return my_malloc(size);
    }

    if (size <= 0)
    {
        my_free(ptr);
        return NULL;
    }

    block = (meta_block_t *)ptr - 1;
    if (block->size >= size)
    {
        // If the existing block is large enough, no need to allocate a new block
        if (block->size > size + sizeof(meta_block_t))
        {
            // Split the block if it's large enough
            new_block = (meta_block_t *)((char *)(block + 1) + size);
            new_block->size = block->size - size - sizeof(meta_block_t);
            new_block->is_free = true;
            new_block->next = block->next;

            block->size = size;
            block->next = new_block;
        }
        return ptr;
    }
    else
    {
        new_ptr = my_malloc(size);
        if (new_ptr == NULL)
        {
            return NULL; // Allocation failed
        }

        // Copy the content of the old block to the new block
        copy_size = block->size < size ? block->size : size;
        memcpy(new_ptr, ptr, copy_size);

        my_free(ptr); // Free the old block
        return new_ptr;
    }
}