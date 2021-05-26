#include "Memory.h"

void* Memory::memcpy(void* destination, const void* source, size_t num)
{
    for(int i = 0; i < num; i++)
        *((uint8_t*)destination + i) = *((uint8_t*)source + i);
    
    return destination;
}

void* Memory::memchr(void* ptr, int value, size_t num)
{
    uint8_t* result = nullptr;

    for(int i = 0; i < num; i++)
        if(*((uint8_t*)ptr + i) == value)
            result = ((uint8_t*)ptr + i);
    
    return result;
}

int Memory::memcmp(const void* ptr1, const void* ptr2, size_t num)
{
    for(int i = 0; i < num; i++)
    {
        uint8_t first = *((uint8_t*)ptr1 + i);
        uint8_t second = *((uint8_t*)ptr2 + i);

        if(first != second)
            return first > second ? 1 : -1;
    }

    return 0;
}

void* Memory::memset(void* ptr, int value, size_t num)
{
    for(int i = 0; i < num; i++)
        *((uint8_t*)ptr + i) = (uint8_t)value;
    
    return ptr;
}