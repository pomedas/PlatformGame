#include "Log.h"

#include <iostream>
#include <cstdarg>
#include <cstdio>

void Log(const char file[], int line, const char* format, ...)
{
    static char tmpString1[4096];
    static va_list ap;

    // Construct the string from variable arguments
    va_start(ap, format);
    vsnprintf(tmpString1, 4096, format, ap);
    va_end(ap);

    // Print the formatted string to std::cout
    std::cout << "\n" << file << "(" << line << ") : " << tmpString1 << std::endl;
}
