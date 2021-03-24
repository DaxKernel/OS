#ifndef MEM_H
#define MEM_H
    #include <kernel/multiboot_32.h>
    void allocate_buffer();
    void parse_available_mem();
    uint32_t* get_page();
    void free_page(uint32_t *page);
    int get_num_free_pages();
#endif