#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/kbd.h>
#include <d_unit/d_unit.h>
#include <kernel/interrupt/interrupts.h>
#include <kernel/gdt.h>
#include <kernel/multiboot_32.h>
#include <kernel/mem/mem.h>
#include <kernel/mem/mmap.h>
#include <kernel/mem/p_stack.h>
#include <kernel/mem/liballoc.h>


// Comment out the follow macro definiton to disable unit tests from running
#define D_UNIT_ENABLED

void print_kernel_end(){
    extern char _kernel_end;
    printf("_kernel_end = %d\n", &_kernel_end);
}

void print_header()
{
    tty_write_string_centered("DAX Operating System");
    tty_write_string_centered("2021 (c)");
    tty_print_horizontal_rule('-');
}

void init_devices()
{
    tty_initialize();
    kbd_init();
}

void init_descriptors()
{
    gdt_init();
    idt_init();
}

void kernel_main(multiboot_info_t* mbt, unsigned int magic)
{
    init_descriptors();
    init_devices();
    init_mem(mbt);
    verify_multiboot(magic, mbt);
    print_header();
#ifdef D_UNIT_ENABLED
    D_UNIT_run();
#endif
}
