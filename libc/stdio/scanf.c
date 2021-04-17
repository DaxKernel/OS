#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <kernel/kbd/kbd_input.h>

int scanf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    while (*format != '\0')
    {
        if (*format == '%')
        {
            int *output_int;
            char option = *(++format), *output_str;
            switch (option)
            {
            case 'd':
                output_int = va_arg(args, int *);
                get_int(output_int);
                break;
            case 's':
                output_str = va_arg(args, char *);
                get_str(output_str);
                break;
            default:
                return -1;
            }
        }
        format++;
    }
    va_end(args);
    return 1;
}