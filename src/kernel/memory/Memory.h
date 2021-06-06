#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <stddef.h>

namespace Memory
{
    void* memcpy(void* destination, const void* source, size_t num);

    void* memchr(void* ptr, int value, size_t num);

    int memcmp(const void* ptr1, const void* ptr2, size_t num);

    void* memset(void* ptr, int value, size_t num);
}

#endif