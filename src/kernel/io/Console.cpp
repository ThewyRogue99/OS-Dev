#include "Console.h"

#define VIDEO_MEMORY_START 0xB8000
#define VIDEO_MEMORY_END 0xB8FA0

namespace OS
{
    Console::Console() { }

    void Console::print(const char* str, ...)
    {
        va_list args;
        va_start(args, str);

        for(int i = 0; i < String::strlen(str); i++)
        {
            char charToPrint = str[i];
            if(charToPrint == '%')
            {
                char nextChar = str[++i];
                if(nextChar == 'i')
                {
                    char buff[11];
                    String::itoa(va_arg(args, int), buff, 10);
                    print(buff);
                }
                else if(nextChar == 's')
                    print(va_arg(args, const char*));
                else
                    print("%");
            }
            else
            {
                *currentVideoPtr = charToPrint;
                currentVideoPtr += 2;
            }
        }

        va_end(args);
    }

    void Console::clear()
    {
        char* console_start = (char*)VIDEO_MEMORY_START;

        for(int i = 0; i < VIDEO_MEMORY_END - VIDEO_MEMORY_START; i += 2)
            *(console_start + i) = 0x0;
    }

    uint8_t Console::getVideoMemoryAddress(int row, int column)
    {
        return VIDEO_MEMORY_START + 2 * (row * 80 + column);
    }

    void Console::setTextColor(ConsoleColors color)
    {
        char* console_start = (char*)VIDEO_MEMORY_START;

        for(int i = 1; i < VIDEO_MEMORY_END - VIDEO_MEMORY_START; i += 2)
        {
            uint8_t* byte = (uint8_t*)(console_start + i);
            *byte = (*byte & 0xF0) | (uint8_t)color;
        }
    }

    void Console::setBackgroundColor(ConsoleColors color)
    {
        char* console_start = (char*)VIDEO_MEMORY_START;

        for(int i = 1; i < VIDEO_MEMORY_END - VIDEO_MEMORY_START; i += 2)
        {
            uint8_t* byte = (uint8_t*)(console_start + i);
            *byte = (*byte & 0x0F) | ((uint8_t)color << 4);
        }
    }
}