#ifndef MULTIBOOT_EXTRA_H
#define MULTIBOOT_EXTRA_H

#include <kernel/multiboot.h>
#include <stdint.h>

typedef struct multiboot_memory_map {
	uint32_t size;
	uint32_t base_addr_low,base_addr_high;
	uint32_t length_low,length_high;
	#define MULTIBOOT_MEMORY_AVAILABLE              1
	#define FREE_MEMORY_OVERLAP              6	
	uint32_t type;
} multiboot_memory_map_t;
 
typedef multiboot_memory_map_t mmap_entry_t;

void verify_multiboot(uint32_t magic, multiboot_info_t* mbt);

#endif