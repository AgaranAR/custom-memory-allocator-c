#include <stdio.h>
#include <stdlib.h>
#include <stdalign.h>
#include <stdint.h>
#include <stddef.h>

#define MEMORY_SIZE 1024

typedef struct Block {
    size_t size;
    int free;
    struct Block* next;
} Block;

static unsigned char memory[MEMORY_SIZE];
static Block* freeList = NULL;

// Align size to system alignment
static size_t align_up(size_t size) {
    size_t align = alignof(max_align_t);
    return (size + align - 1) & ~(align - 1);
}

// Initialize memory pool
void init_memory(void) {
    freeList = (Block*)memory;
    freeList->size = MEMORY_SIZE - sizeof(Block);
    freeList->free = 1;
    freeList->next = NULL;
}

// Split block if large enough
static void split(Block* block, size_t size) {
    size_t alignedSize = align_up(size);

    if (block->size < alignedSize + sizeof(Block) + 1U) {
        return;
    }

    Block* newBlock = (Block*)((unsigned char*)block + sizeof(Block) + alignedSize);
    newBlock->size = block->size - alignedSize - sizeof(Block);
    newBlock->free = 1;
    newBlock->next = block->next;

    block->size = alignedSize;
    block->free = 0;
    block->next = newBlock;
}

// Allocate memory
void* my_malloc(size_t size) {
    if (size == 0 || size > MEMORY_SIZE - sizeof(Block)) {
        return NULL;
    }

    size_t alignedSize = align_up(size);
    Block* curr = freeList;

    while (curr) {
        if (curr->free && curr->size >= alignedSize) {

            if (curr->size >= alignedSize + sizeof(Block) + 1U) {
                split(curr, alignedSize);
            }

            curr->free = 0;
            printf("Allocated %zu bytes\n", size);

            return (void*)((unsigned char*)curr + sizeof(Block));
        }
        curr = curr->next;
    }

    printf("Memory allocation failed\n");
    return NULL;
}

// Merge adjacent free blocks
void merge(void) {
    Block* curr = freeList;

    while (curr && curr->next) {
        if (curr->free && curr->next->free) {
            curr->size += curr->next->size + sizeof(Block);
            curr->next = curr->next->next;
            continue;
        }
        curr = curr->next;
    }
}

// Free memory
void my_free(void* ptr) {
    if (!ptr) return;

    // Safety check
    if ((unsigned char*)ptr < memory || (unsigned char*)ptr >= memory + MEMORY_SIZE) {
        return;
    }

    Block* block = (Block*)((unsigned char*)ptr - sizeof(Block));
    block->free = 1;

    printf("Freed memory block\n");
    merge();
}

// Print memory layout
void print_memory(void) {
    Block* curr = freeList;
    printf("\nMemory Layout:\n");

    while (curr) {
        printf("[Size: %zu | Free: %d] -> ", curr->size, curr->free);
        curr = curr->next;
    }
    printf("NULL\n");
}

// Print stats (🔥 upgrade)
void print_stats(void) {
    Block* curr = freeList;
    size_t freeMem = 0, usedMem = 0;

    while (curr) {
        if (curr->free) freeMem += curr->size;
        else usedMem += curr->size;
        curr = curr->next;
    }

    printf("\n[STATS] Used: %zu bytes | Free: %zu bytes\n", usedMem, freeMem);
}

// Test program
int main(void) {
    init_memory();

    void* a = my_malloc(200);
    void* b = my_malloc(100);

    print_memory();
    print_stats();

    my_free(a);

    print_memory();
    print_stats();

    void* c = my_malloc(50);

    print_memory();
    print_stats();

    (void)b;
    (void)c;

    return 0;
}