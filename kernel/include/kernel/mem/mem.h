#ifndef MEM_H
#define MEM_H

#include <kernel/multiboot_32.h>
#include <stddef.h>

void allocate_buffer();
void parse_available_mem();
uint32_t* get_page();
void free_page(uint32_t *page);
size_t get_num_free_pages();
void init_mem(multiboot_info_t* mbt);

#endif