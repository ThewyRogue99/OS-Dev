#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include "../io/Console.h"
#include "Memory.h"

namespace Memory
{
    template<typename T>
    class Stack
    {
    public:
        Stack() { }

        Stack(uint8_t* start_address, uint8_t* end_address, bool debug = true)
            : start_address(start_address), end_address(end_address), stack_pointer(start_address), debug(debug) { }

        Stack(uint8_t* start_address, int max_item_count, bool debug = true)
            : start_address(start_address), end_address((uint8_t*)((sizeof(T)* max_item_count) + (uint32_t)start_address)),
            stack_pointer(start_address), debug(debug) { }

        void push(T item)
        {
            if (isEmpty) isEmpty = false;

            memcpy(stack_pointer, &item, sizeof(item));

            stack_pointer += sizeof(item);
        }

        T pop()
        {
            if (isEmpty) { /*if (debug) Console::print("Error: Attempted to read a non-existing value from %s", name);*/ return 0; }

            T result;

            stack_pointer -= sizeof(T);

            memcpy(&result, stack_pointer, sizeof(T));

            memset(stack_pointer, 0, sizeof(T));

            isEmpty = stack_pointer == start_address;

            return result;
        }

        T copypop()
        {
            if (isEmpty) { /*if (debug) Console::print("Error: Attempted to read a non-existing value from %s", name);*/ return 0; }

            T result;

            uint8_t* pointer = stack_pointer - sizeof(T);

            memcpy(&result, pointer, sizeof(T));

            return result;
        }

        void changeName(const char* newName) { name = newName; }

        uint8_t* start_address = nullptr;
        uint8_t* end_address = nullptr;
        uint8_t* stack_pointer = nullptr;
        
        const char* name = "Kernel Stack";

    private:
        bool isEmpty = true;

        bool debug = true;
    };
}

#endif