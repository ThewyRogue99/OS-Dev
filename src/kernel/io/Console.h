#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
#include <stdint.h>
#include "../types/String.h"

enum class ConsoleColors
{
    Black,
    Blue,
    Green,
    Cyan,
    Red,
    Purple,
    Brown,
    Gray,
    Dark_Gray,
    Light_Blue,
    Light_Green,
    Light_Cyan,
    Light_Red,
    Light_Purple,
    Yellow,
    White
};

namespace OS
{
    class Console
    {
    public:
        Console();

        void print(const char* str, ...);

        void clear();

        void setTextColor(ConsoleColors color);

        void setBackgroundColor(ConsoleColors color);
    protected:
        char* currentVideoPtr = (char*)0xb8000;

        uint8_t getVideoMemoryAddress(int row, int column);
    };
}

#endif