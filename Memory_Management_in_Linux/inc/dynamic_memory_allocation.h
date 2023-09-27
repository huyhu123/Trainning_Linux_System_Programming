#ifndef _DYNAMIC_MEMORY_ALLOCATION_H_
#define _DYNAMIC_MEMORY_ALLOCATION_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

typedef struct meta_block
{
    int size;
    struct meta_block *next;
    bool is_free;
} meta_block_t;

void *my_malloc(size_t size);

void my_free(void *ptr);

void *my_realloc(void *ptr, size_t size);

#endif