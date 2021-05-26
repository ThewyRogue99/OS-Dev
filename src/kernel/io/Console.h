#ifndef CONSOLE_H
#define CONSOLE_H

#include <stdarg.h>
#include <stdint.h>
#include "../types/String.h"
#include "io.h"

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
        static void print(const char* str, ...);

        static void clear();

        static void setTextColor(ConsoleColors color);

        static void setBackgroundColor(ConsoleColors color);

        static void deleteLastCharacter();

        static void moveCursor(int val);

        static void update();
    protected:
        static uint8_t* currentVideoPtr;

        static uint8_t getVideoMemoryAddress(int row, int column);

        static void update_cursor(int x, int y);

        static void update_cursor(uint16_t index);

        static uint8_t* getCursorAddress();

    private:
        static uint16_t cursor_index;
    };
}

#endif