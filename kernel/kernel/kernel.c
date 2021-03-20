#include <stdio.h>
#include <kernel/tty.h>
#include <kernel/kbd.h>
#include <kernel/kbd_input.h>
#include <d_unit/d_unit.h>
#include <kernel/interrupts.h>
#include <kernel/gdt.h>
#include <stdlib.h>
#include <kernel/multiboot_32.h>

// Comment out the follow macro definiton to disable unit tests from running
#define D_UNIT_ENABLED



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
    print_header();
#ifdef D_UNIT_ENABLED
    D_UNIT_run();
#endif
    mmap_entry_t* entry = mbt->mmap_addr;
    int i=0; int x;
	while(entry < mbt->mmap_addr + mbt->mmap_length) {
		// do something with the entry
		entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
        printf("Entry %d\n", i+1);
        printf("size = %d\n", entry->size);
        printf("Base address low = %d, high = %d\n", entry->base_addr_low, entry->base_addr_high);
        printf("Length low = %d, high = %d\n", entry->length_low, entry->length_high);
        printf("Type = %d\n", entry->type);
        scanf("%d", &x);
	}
}
