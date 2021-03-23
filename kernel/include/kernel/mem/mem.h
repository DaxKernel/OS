#ifndef MEM_H
#define MEM_H
    #include <kernel/multiboot_32.h>
    void allocate_buffer();
    void parse_available_mem(multiboot_info_t* mbt);
    void print_free_list();
    int* get_page();
#endif