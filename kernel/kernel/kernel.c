#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/kbd.h>
#include <d_unit/d_unit.h>
#include <kernel/interrupts.h>

// Comment out the follow macro definiton to disable unit tests from running
// #define D_UNIT_ENABLED

void print_header()
{
    tty_write_string_centered("DAX Operating System");
    tty_write_string_centered("Build 1.0 2020 (c)");
    tty_print_horizontal_rule('-');
}

void init_devices()
{
    tty_initialize();
    kbd_init();
}

void kernel_main(void)
{
    idt_init();
    init_devices();
    print_header();
#ifdef D_UNIT_ENABLED
    D_UNIT_run();
#endif
}
