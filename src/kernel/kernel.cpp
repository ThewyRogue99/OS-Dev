#include "io/Console.h"
#include "io/Keyboard.h"
#include "memory/Stack.h"
#include "memory/Memory.h"
#include "memory/Heap.h"
#include "kernel.h"
#include "types/String.h"

using namespace OS;
using namespace Memory;

void keyCallback(int state, int scan_code, int key_code);

Heap KernelHeap;
Stack<uint8_t> KernelStack;

void* operator new(size_t size) { return KernelHeap.malloc(size); }
void* operator new[](size_t size) { return KernelHeap.malloc(size); }
void operator delete(void* ptr, unsigned int) { KernelHeap.free(ptr); }

KERNEL_MAIN
{
    Console::clear();
    Console::setTextColor(ConsoleColors::White);
    Console::setBackgroundColor(ConsoleColors::Blue);

    KernelStack = Stack<uint8_t>((uint8_t*)KERNEL_STACK_START, (uint8_t*)KERNEL_STACK_END);
    KernelHeap = Heap((uint8_t*)KERNEL_HEAP_START, (uint8_t*)KERNEL_HEAP_END);
    KernelHeap.initHeap();

    String str = "Hello, World!";
    str += "\nHow are You?";

    Console::print("%s\n", str.c_str());

    Keyboard::registerKeyCallback(keyCallback);

    KERNEL_MAIN_LOOP
    {
        Keyboard::handleKey();
        Console::update();
    }
}

void keyCallback(int state, int scan_code, int key_code)
{
    if(state == STATE_PRESS)
    {
        if(key_code == '\b')
            Console::deleteLastCharacter();
        else
            Console::print("%c", key_code);
    }
}