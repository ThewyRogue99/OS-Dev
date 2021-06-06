#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>
#include "Memory.h"
#include "../io/Console.h"

namespace Memory
{
    class Heap
    {
    public:
        uint8_t* startAddress;
        size_t heapSize;
        uint8_t* endAddress;

        Heap() { }

        Heap(uint8_t* startAddress, size_t heapSize)
            : startAddress(startAddress), heapSize(heapSize),
            endAddress(startAddress + heapSize) { }

        Heap(uint8_t* startAddress, uint8_t* endAddress)
            : startAddress(startAddress), endAddress(endAddress),
            heapSize(endAddress - startAddress) { }

        Heap(uint8_t* startAddress, size_t heapSize, const char* name)
            : startAddress(startAddress), heapSize(heapSize),
            endAddress(startAddress + heapSize), name(name) { }

        Heap(uint8_t* startAddress, uint8_t* endAddress, const char* name)
            : startAddress(startAddress), endAddress(endAddress),
            heapSize(endAddress - startAddress), name(name) { }

        bool initHeap();

        void setName(const char* newName) { name = newName; }

        void* malloc(size_t size);

        void* calloc(size_t num, size_t size) { return malloc(num * size); }

        void* realloc(void* ptr, size_t size);

        void free(void* ptr);

        void debug();

    private:
        const char* name = "Heap";

        typedef struct HeapBlock
        {
            HeapBlock* next;
            bool isUsed;
            size_t size;
        }HeapBlock;

        uint8_t* writeBlock(HeapBlock block, uint8_t* address);

        HeapBlock* findFreeBlock(size_t size);

        void concatBlocks(HeapBlock* block, bool resetData = false);
        
        HeapBlock* getBlock(uint8_t* address);
    };
}

#endif