#include <kernel/c_buffer.h>
#include <stdbool.h>

void get_str(char *str)
{
    reset_buffer();
    char t = '\0';
    char *ptr = str;
    while (true)
    {
        if (!is_buffer_empty())
        {
            t = read_buffer();
            if (t != '\n')
            {
                *ptr = t;
                ++ptr;
            }
            else
                break;
        }
    }
    *ptr = '\0';
}