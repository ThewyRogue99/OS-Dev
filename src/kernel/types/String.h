#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include "macros.h"
#include "../memory/Memory.h"

class String
{
public:
    	String() { }

	String(const char* str) : _str(str) { }

	String(const char* str, size_t length)
	{
		_str = new char[length + 1];

		strncpy((char*)_str, str, length);
	}

	// Numbers
	String(int val);

	// Char
	String(char val);

	// Functions
	String operator+(char val);
	String operator+(String str);
	String operator+(const char* str);

	void operator+=(char val);
	void operator+=(String str);
	void operator+=(const char* str);

	const char* c_str() { return _str; }

	size_t length() { return strlen(_str); }

	void clear();

    static void swap(char *x, char *y);

    static char* itoa(int value, char* buffer, int base);

    static size_t strlen(const char* str);

    static char* strcpy(char* destination, const char* source);

    static char* strncpy(char* destination, const char* source, size_t num);

    static char* reverse(char *buffer, int i, int j);

protected:
    const char* _str;
};

#endif