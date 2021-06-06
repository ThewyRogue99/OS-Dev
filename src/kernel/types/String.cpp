#include "String.h"

String::String(int val)
{
	char buff[20];
    itoa(val, buff, 10);

	clear();
	_str = new char[strlen(buff)];

	strcpy((char*)_str, buff);
}

String::String(char val)
{
	clear();
	char* ptr = new char[2];

	ptr[0] = val;
	ptr[1] = '\0';

	_str = ptr;
}

String String::operator+(char val)
{
	size_t strLen = length();

	char* ptr = new char[strLen + 2];
	Memory::memcpy(ptr, _str, strLen);
	clear();

	ptr[strLen] = val;
	ptr[strLen + 1] = '\0';

	_str = ptr;

	return *this;
}

String String::operator+(String str)
{
	return operator+(str.c_str());
}

String String::operator+(const char* str)
{
	size_t strLen = length();
	char* ptr = new char[strLen + strlen(str) + 1];

	Memory::memcpy(ptr, _str, strLen);
	strcpy(ptr + strLen, str);

	//TODO: Clear function aborts program
	clear();

	_str = ptr;

	return *this;
}

void String::operator+=(char val)
{
	operator+(val);
}

void String::operator+=(String str)
{
    operator+(str);
}

void String::operator+=(const char* str)
{
    operator+(str);
}

void String::clear()
{
	if (_str != nullptr)
	{
		delete _str;
		_str = nullptr;
	}
}

char* String::strcpy(char* destination, const char* source)
{
    size_t len = strlen(source);

    for(size_t i = 0; i < len; i++)
        destination[i] = source[i];
    
    destination[len] = '\0';

    return destination;
}

char* String::strncpy(char* destination, const char* source, size_t num)
{
    bool padding = false;

    for(size_t i = 0; i < num; i++)
    {
        if(source[i] == '\0') padding = true;

        destination[i] = padding ? 0 : source[i];
    }

    return destination;
}

void String::swap(char *x, char *y) 
{
    char t = *x; *x = *y; *y = t;
}

size_t String::strlen(const char* str)
{
    size_t i;
    for(i = 0; str[i] != '\0'; i++);
    return i;
}

char* String::reverse(char *buffer, int i, int j)
{
    while (i < j) {
        swap(&buffer[i++], &buffer[j--]);
    }
 
    return buffer;
}

char* String::itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32) {
        return buffer;
    }
 
    // consider the absolute value of the number
    int n = abs(value);
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) {
            buffer[i++] = 65 + (r - 10);
        }
        else {
            buffer[i++] = 48 + r;
        }
 
        n = n / base;
    }
 
    // if the number is 0
    if (i == 0) {
        buffer[i++] = '0';
    }
 
    // If the base is 10 and the value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10) {
        buffer[i++] = '-';
    }
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse(buffer, 0, i - 1);
}