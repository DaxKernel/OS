#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/kbd.h>
#include <stdbool.h>

void print_header()
{
    terminal_write_string_centered("DAX Operating System");
    terminal_write_string_centered("Build 1.0 2020 (c)");
    terminal_print_horizontal_rule('-');
}

void kernel_main(void)
{
    terminal_initialize();
    print_header();
    if (kbd_initialize())
    {
        terminal_writestring("Keyboard Status - [");
        terminal_print_success("OK");
        terminal_writestring("]\n");
    }
    kbd_set_leds(true, true, true);
    kbd_draw();
}
