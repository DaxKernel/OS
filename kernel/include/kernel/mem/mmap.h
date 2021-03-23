#ifndef MMAP_H
#define MMAP_H
    #include <kernel/multiboot_32.h>
    extern multiboot_info_t* mbt;
    void mmap_iterate(void (*)(mmap_entry_t*));
    void set_multiboot_info_t(multiboot_info_t* m);
#endif