#include <kernel/mem/mem.h>
#include <kernel/mem/p_stack.h>
#include <stdio.h>

static multiboot_info_t* mbt;

extern char _kernel_end;

// Size of each page frame in KB
static const int frame_size = 4 * 1024;

static const uint32_t MB = 1024 * 1024;

static int stack_size = -1;

void allocate_buffer(){  
  init_stack(stack_size);
  mmap_entry_t* entry = (mmap_entry_t *)mbt->mmap_addr;
  while(entry < mbt->mmap_addr + mbt->mmap_length) {
    if(entry->type == MULTIBOOT_MEMORY_AVAILABLE && entry->base_addr_low >= MB){
      for(int j=0; j<stack_size; ++j)
        push((char *)entry->base_addr_low + j*frame_size);
    }
    entry = (mmap_entry_t*) ((uint32_t) entry + entry->size + sizeof(entry->size));
  }
}



void parse_available_mem(multiboot_info_t* mbt){
  mmap_entry_t* entry = (mmap_entry_t *)mbt->mmap_addr;
  uint32_t total_bytes = 0;
	while(entry < mbt->mmap_addr + mbt->mmap_length) {
    /* Does a free block of memory overlap with the region in memory
         where we have the kernel and stack stored? 
         If so, lets mutate the struct so that the free region starts from 
         the end of the stack.
      */
    if(entry->type == MULTIBOOT_MEMORY_AVAILABLE && entry->base_addr_low >= MB){
      
      if(entry->base_addr_low < &_kernel_end){
        // Calculations for adjusting kernel in memory.
        void *first_start = &_kernel_end;
        uint32_t first_length = entry->length_low - (uint32_t)((char *)first_start - (char *)entry->base_addr_low);

        // Calculations for adjusting stack in memory.
        void *second_start = (uint32_t *)first_start + (first_length / frame_size);
        uint32_t second_length = first_length - (uint32_t)((char *)second_start - (char *)first_start);

        // TODO: This isn't precisely correct. We are kind of overallocating on the stack.
        entry->base_addr_low = (uint32_t)second_start;     
        entry->length_low = second_length;
    
      }
      total_bytes += entry->length_low;
    }
		entry = (mmap_entry_t*) ((uint32_t) entry + entry->size + sizeof(entry->size));
	}
  stack_size = total_bytes / frame_size;
}


void print_grub_mmap(){
  mmap_entry_t* entry = (mmap_entry_t *)mbt->mmap_addr;
  int i=0, x;
	while(entry < mbt->mmap_addr + mbt->mmap_length) {
        printf("Mem location #%d\n", ++i);
        printf("base_low = %d base_high=%d\n", entry->base_addr_low, entry->base_addr_high);
        printf("length_low = %d, length_high = %d\n", entry->length_low, entry->length_high);
        printf("Type = %d\n\n", entry->type);
        scanf("%d", &x);
        entry = (mmap_entry_t*) ((uint32_t) entry + entry->size + sizeof(entry->size));
	}
}

void set_multiboot_info_t(multiboot_info_t* m){
  mbt = m;
}

uint32_t* get_page(){
  return (uint32_t *)pop();
}

void free_page(uint32_t *page){
  push((uint32_t)page);
}

