#ifndef MULTIBOOT_EXTRA_H
#define MULTIBOOT_EXTRA_H

#include <kernel/multiboot.h>

typedef struct multiboot_memory_map {
	unsigned int size;
	unsigned int base_addr_low,base_addr_high;
	unsigned int length_low,length_high;
	#define MULTIBOOT_MEMORY_AVAILABLE              1
	#define MULTIBOOT_MEMORY_RESERVED               2
	#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE       3
	#define MULTIBOOT_MEMORY_NVS                    4
	#define MULTIBOOT_MEMORY_BADRAM                 5
	unsigned int type;
} multiboot_memory_map_t;
 
// this is really an entry, not the entire map.
typedef multiboot_memory_map_t mmap_entry_t;
#endif