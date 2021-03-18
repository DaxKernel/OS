#include <stdlib.h>
#include <stdio.h>

char *itoa(int value, char *str)
{
    // This function is not part of ANSI C
    // But converting integers to string seems to be something commonly required
    int digit;
    int top = -1;
    char s[10];
    char *ptr = str;
    if (value < 0)
    {
        // Number is negative
        *ptr = '-';
        ptr++;
        value = value * -1;
    }
    
    if(value == 0){
        *str = '0';
        *(str+1) = '\0';
        return str;
    }

    while (value > 0 && top <= 10)
    {
        digit = value % 10;
        s[++top] = digit + '0';
        value /= 10;
    }

    while (top >= 0)
    {
        *ptr = s[top--];
        ptr++;
    }
    *ptr = '\0';
    return str;
}