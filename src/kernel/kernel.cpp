#include "io/Console.h"

void main()
{
    OS::Console console;

    console.clear();
    console.setTextColor(ConsoleColors::White);
    console.setBackgroundColor(ConsoleColors::Blue);

    console.print("Hello i am %s and i am %i years old", "Josh", 25);
}