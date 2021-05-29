#include <stdio.h>
#include <kernel/display/tty.h>
#include <kernel/kbd/device.h>
#include <d_unit/d_unit.h>
#include <kernel/interrupt/interrupts.h>
#include <kernel/gdt.h>
#include <kernel/multiboot_32.h>
#include <kernel/mem/mem.h>
#include <kernel/repl.h>

// Comment out the follow macro definiton to disable unit tests from running
#define D_UNIT_ENABLED

void init_devices(multiboot_info_t *mbt)
{
    tty_initialize(mbt);
    kbd_init();
}

void init_descriptors()
{
    gdt_init();
    idt_init();
}

void kernel_main(multiboot_info_t *mbt, unsigned int magic)
{
    init_descriptors();
    init_mem(mbt);
    init_devices(mbt);
    verify_multiboot(magic, mbt);
    tty_print_header();
#ifdef D_UNIT_ENABLED
    D_UNIT_run();
#endif
    accept_commands();
    //draw_image();
}
