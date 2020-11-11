#include <string.h>

const char NUL = '\0';

char *strcpy(char *destination, const char *source)
{
    while (*source != NUL)
    {
        *destination = *source;
        destination++;
        source++;
    }
    return destination;
}