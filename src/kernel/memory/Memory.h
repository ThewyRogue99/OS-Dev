#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

#define KERNEL_START 0x7e00
#define BOOT_SECTOR_END 0x100000
#define KERNEL_MEMORY_START BOOT_SECTOR_END
#define KERNEL_MEMORY_END 0x81000000

namespace Memory
{
    typedef struct kernel_heap_block
    {
        struct kernel_heap_block *next;
        uint32_t size;
        uint32_t used;
        uint32_t bsize;
    }KernelHeapBlock;

    void* memcpy(void* destination, const void* source, size_t num);

    void* memchr(void* ptr, int value, size_t num);

    int memcmp(const void* ptr1, const void* ptr2, size_t num);

    void* memset(void* ptr, int value, size_t num);
}

#endif