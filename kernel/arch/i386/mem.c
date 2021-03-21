#include <kernel/mem.h>

#include <stdio.h>

extern char _kernel_end;

typedef struct {
  // Start of free block of memory
  void *start;
  // Length of this block
  unsigned int length;
} free_mem;

int num_free_mem;
free_mem free_mem_list[10];

void allocate_buffer(){
  /* 
  Use this function to allocate memory at the end of the
  kernel
  This memory is to be used to track the free/used pages
  */
}

void parse_available_mem(multiboot_info_t* mbt){
  mmap_entry_t* entry = (mmap_entry_t *)mbt->mmap_addr;
  int i=0;
	while(entry < mbt->mmap_addr + mbt->mmap_length) {
		entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
    if(entry->type == MULTIBOOT_MEMORY_AVAILABLE){
      // Found a free block of memory; lets parse it.
      free_mem_list[i].start = (void *)entry->base_addr_low;
      free_mem_list[i].length = entry->length_low;
      i++;
    }
	}
  num_free_mem = i;
}

void print_free_list(){
  for(int i=0; i<num_free_mem; ++i){
    printf("Free block starts at %d\n", free_mem_list[i].start);
    printf("Current data is %d\n", *((char *)free_mem_list[i].start));
    printf("Length is %d\n", free_mem_list[i].length);
  }
}

void print_grub_mmap(multiboot_info_t* mbt){
  mmap_entry_t* entry = (mmap_entry_t *)mbt->mmap_addr;
  int i=0, x;
	while(entry < mbt->mmap_addr + mbt->mmap_length) {
		entry = (mmap_entry_t*) ((unsigned int) entry + entry->size + sizeof(entry->size));
        printf("Mem location #%d\n", ++i);
        printf("base_low = %d base_high=%d\n", entry->base_addr_low, entry->base_addr_high);
        printf("length_low = %d, length_high = %d\n", entry->length_low, entry->length_high);
        printf("Type = %d\n\n", entry->type);
        scanf("%d", &x);
	}
}