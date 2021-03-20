#ifndef MULTIBOOT_EXTRA_H
#define MULTIBOOT_EXTRA_H

#include <kernel/multiboot.h>

typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
	unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
	unsigned int type;
} multiboot_memory_map_t;
 
// this is really an entry, not the entire map.
typedef multiboot_memory_map_t mmap_entry_t;
#endif