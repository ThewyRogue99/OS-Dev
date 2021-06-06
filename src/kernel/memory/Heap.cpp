#include "Heap.h"
#include "../kernel.h"

using namespace OS;
using namespace Memory;

bool Heap::initHeap()
{
    if (heapSize < 13)
    {
        Console::print("Heap size is too small to initialize!\n");
        return false;
    }

    writeBlock({ nullptr, false, heapSize - sizeof(HeapBlock) }, startAddress);

    return true;
}

uint8_t* Heap::writeBlock(HeapBlock block, uint8_t* address)
{
    if (!address)
        return nullptr;

    memcpy(address, &block, sizeof(block));

    return address + sizeof(block);
}

Heap::HeapBlock* Heap::findFreeBlock(size_t size)
{
    if (!startAddress)
        return nullptr;

    HeapBlock* ptr = (HeapBlock*)startAddress;

    do
    {
        if (!ptr->isUsed && size <= ptr->size)
        {
            concatBlocks(ptr);
            return ptr;
        }

        ptr = ptr->next;
    } while (ptr != nullptr);

    return nullptr;
}

void Heap::debug()
{
    HeapBlock* ptr = (HeapBlock*)startAddress;

    do
    {
        Console::print("--- Heap Block ---\n");
        Console::print("Block Start: %i\n", ptr);
        Console::print("Next: %s\n", ptr->next != nullptr ? "Available" : "NULL");
        Console::print("Is Used: %s\n", ptr->isUsed ? "true" : "false");
        Console::print("Size: %i\n", ptr->size);
        Console::print("Start Address: %i\n", (uint8_t*)ptr + sizeof(HeapBlock));
        Console::print("------------------\n\n");

        ptr = ptr->next;
    } while (ptr != nullptr);
}

void* Heap::malloc(size_t size)
{
    HeapBlock* freeBlock = findFreeBlock(size);

    if (!freeBlock)
        return nullptr;

    if (freeBlock->size != size)
    {
        int nextBlockSize = freeBlock->size - size - sizeof(HeapBlock);
        uint8_t* nextBlockAddress = (uint8_t*)freeBlock + sizeof(HeapBlock) + size;

        HeapBlock next = { freeBlock->next, false, (size_t)nextBlockSize };

        writeBlock(next, nextBlockAddress);
        freeBlock->next = (HeapBlock*)nextBlockAddress;
        freeBlock->size = size;
        freeBlock->isUsed = true;
    }
    else
        freeBlock->isUsed = true;

    return (uint8_t*)freeBlock + sizeof(HeapBlock);
}

Heap::HeapBlock* Heap::getBlock(uint8_t* address)
{
    if (!address)
        return nullptr;

    HeapBlock* ptr = (HeapBlock*)startAddress;

    do
    {
        if ((uint8_t*)ptr <= address && ((uint8_t*)ptr + sizeof(HeapBlock) + ptr->size) > address)
            return ptr;

        ptr = ptr->next;
    } while (ptr != nullptr);

    return nullptr;
}

void Heap::concatBlocks(HeapBlock* block, bool resetData)
{
    if (!block) return;

    size_t totalSize = 0;

    HeapBlock* ptr = block->next;
    HeapBlock* nextCopy = ptr;

    while (ptr != nullptr && !ptr->isUsed)
    {
        totalSize += ptr->size + sizeof(HeapBlock);

        if (ptr) nextCopy = ptr->next;
        ptr = ptr->next;
    }

    uint8_t* addr = resetData ? (uint8_t*)block + sizeof(HeapBlock) : (uint8_t*)block->next;
    size_t memsetSize = resetData ? totalSize + block->size : totalSize;

    block->size += totalSize;
    block->next = nextCopy;

    if(memsetSize > 0)
        memset(addr, 0, memsetSize);
}

void* Heap::realloc(void* ptr, size_t size)
{
    if (!ptr)
        return nullptr;

    HeapBlock* block = getBlock((uint8_t*)ptr);

    if (!block)
        return nullptr;

    uint8_t* blockData = (uint8_t*)malloc(block->size);
    size_t blockSize = block->size;

    memcpy(blockData, block + sizeof(HeapBlock), block->size);

    block->isUsed = false;
    concatBlocks(block);

    uint8_t* result = (uint8_t*)malloc(size);
    memcpy(result, blockData, blockSize);
    free(blockData);

    return result;
}

void Memory::Heap::free(void* ptr)
{
    if (!ptr) return;

    HeapBlock* block = getBlock((uint8_t*)ptr);

    if (!block) return;

    block->isUsed = false;
    concatBlocks(block, true);
}