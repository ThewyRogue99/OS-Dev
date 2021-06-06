#ifndef KERNEL_H
#define KERNEL_H

#include "memory/Heap.h"
#include "memory/Stack.h"

// Definitions
#define KERNEL_MAIN void main()
#define KERNEL_MAIN_LOOP while(true)

// Address
#define KERNEL_START 0x7e00
#define BOOT_SECTOR_END 0x100000

// Size
#define KERNEL_HEAP_SIZE 0x500000
#define KERNEL_STACK_SIZE KERNEL_HEAP_SIZE

// Address
#define KERNEL_MEMORY_START BOOT_SECTOR_END
#define KERNEL_STACK_START KERNEL_MEMORY_START
#define KERNEL_STACK_END (KERNEL_STACK_START + KERNEL_STACK_SIZE)
#define KERNEL_HEAP_START (KERNEL_STACK_END + 500)
#define KERNEL_HEAP_END (KERNEL_HEAP_START + KERNEL_HEAP_SIZE)


extern Memory::Heap KernelHeap;
extern Memory::Stack<uint8_t> KernelStack;

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr);

#endif