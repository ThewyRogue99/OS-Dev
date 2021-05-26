#include "Console.h"

#define VIDEO_MEMORY_START 0xB8000
#define VIDEO_MEMORY_END 0xB8FA0

uint8_t* OS::Console::currentVideoPtr = (uint8_t*)VIDEO_MEMORY_START;

uint16_t OS::Console::cursor_index = 0;

namespace OS
{
    void Console::update()
    {
        cursor_index = ((int)currentVideoPtr - VIDEO_MEMORY_START) / 2;
        update_cursor(cursor_index);
    }

    void Console::moveCursor(int val)
    {
        uint8_t* futureCursorPtr = getCursorAddress() + val * 2;
        if(futureCursorPtr < currentVideoPtr)
            cursor_index += val;
    }

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
                else if(nextChar == 'c')
                {
                    const char _str[] = { (char)va_arg(args, int), '\0' };
                    print(_str);
                }
                else
                    print("%");
            }
            else if(charToPrint == '\n')
		        currentVideoPtr += 0xA0 - ((uint32_t)currentVideoPtr - VIDEO_MEMORY_START) % 0xA0;
	        else if(charToPrint == '\r')
		        currentVideoPtr -= ((uint32_t)currentVideoPtr - VIDEO_MEMORY_START) % 0xA0;
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

    void Console::deleteLastCharacter()
    {
        uint8_t* videoPtrCopy = currentVideoPtr - 2;

        if((int)videoPtrCopy >= VIDEO_MEMORY_START)
        {
            currentVideoPtr -= 2;
            *currentVideoPtr = 0;
        }
    }

    uint8_t* Console::getCursorAddress()
    {
        return (uint8_t*)(cursor_index * 2 + VIDEO_MEMORY_START);
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

    void Console::update_cursor(int x, int y)
    {
        uint16_t pos = y * 80 /*Text-Mode width*/ + x;
    
        IO::port_byte_out(0x3D4, 0x0F);
        IO::port_byte_out(0x3D5, (uint8_t) (pos & 0xFF));

        IO::port_byte_out(0x3D4, 0x0E);
        IO::port_byte_out(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    }

    void Console::update_cursor(uint16_t index)
    {
        IO::port_byte_out(0x3D4, 0x0F);
        IO::port_byte_out(0x3D5, (uint8_t) (index & 0xFF));

        IO::port_byte_out(0x3D4, 0x0E);
        IO::port_byte_out(0x3D5, (uint8_t) ((index >> 8) & 0xFF));
    }
}