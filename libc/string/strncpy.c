#include <string.h>

const char NUL = '\0';

char *strncpy(char *destination, const char *source, size_t num)
{
    while (*source != NUL)
    {
        *destination = *source;
        source++;
        destination++;
        num--;
    }
    // If we reached end of source before writing num bytes, pad with zeros.
    while (num > 0)
    {
        *destination = NUL;
        destination++;
        num--;
    }
    return destination;
}