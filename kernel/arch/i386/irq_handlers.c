#include <kernel/io.h>
#include <kernel/kbd.h>
#include <stdio.h>

void keyboard_handler_main(void)
{
    /* write EOI */
    outportb(0x20, 0x20);

    kbd_draw();
}