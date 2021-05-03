#ifndef STRING_H
#define STRING_H

#include "macros.h"

class String
{
public:
    static void swap(char *x, char *y);

    static char* itoa(int value, char* buffer, int base);

    static int strlen(const char* str);

    static char* reverse(char *buffer, int i, int j);

protected:
    const char* _str;
};

#endif