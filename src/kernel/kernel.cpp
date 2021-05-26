#include "io/Console.h"
#include "io/Keyboard.h"
#include "memory/KernelStack.h"
#include "memory/Memory.h"

using namespace OS;

void keyCallback(int state, int scan_code, int key_code);

void main()
{
    Console::clear();
    Console::setTextColor(ConsoleColors::White);
    Console::setBackgroundColor(ConsoleColors::Blue);

    Keyboard::registerKeyCallback(keyCallback);

    Memory::KernelStack<int> kernel_stack((uint8_t*)KERNEL_MEMORY_START, 100);

    kernel_stack.push(15);
    kernel_stack.push(20);
    Console::print("1st: %i   2st: %i\n", kernel_stack.pop(), kernel_stack.pop());

    while(true)
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