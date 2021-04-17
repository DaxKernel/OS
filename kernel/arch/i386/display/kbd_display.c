/*
We want the keyboard character to be displayed on the screen.
We can use the optional callback.
*/
#include <kernel/kbd_callback.h>

#ifdef ENABLE_KBD_CALLBACK

#include <stdio.h>
void callback(char code)
{
    printf("%c", code);
}

#endif