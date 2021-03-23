#include <kernel/mem/mem.h>
#include <kernel/mem/p_stack.h>
#include <stdio.h>

extern char _kernel_end;

// Size of each page frame in KB
const int frame_size = 4 * 1024;

typedef struct {
  // Start of free block of memory
  void *start;
  // Length of this block
  unsigned int length;
} free_mem;

int num_free_mem;
free_mem free_mem_list[10];

// Get the size of the stack needed in bytes
int get_stack_size(){
  int sum=0;
  for(int i=0; i<num_free_mem; ++i){
    sum += free_mem_list[i].length;
  }
  return sum/frame_size;
}

void print(int i){
  printf("start = %d\n", free_mem_list[i].start);
  printf("length = %d\n\n", free_mem_list[i].length);
}

void correct_mem(){
  for(int i=0; i<num_free_mem; ++i){
    if(free_mem_list[i].start < &_kernel_end){

      // Calculations for adjusting kernel in memory
      void *first_start = &_kernel_end;
      int first_length = free_mem_list[i].length - (int)((char *)first_start - (char *)free_mem_list[i].start);

      // Calculations for adjusting stack in memory
      void *second_start = (int *)first_start + (first_length / frame_size);
      int second_length = first_length - (int)((char *)second_start - (char *)first_start);

      print(i);
      free_mem_list[i].start = second_start;
      free_mem_list[i].length = second_length;
      print(i);
    }
  }
}

void allocate_buffer(){
  correct_mem();
  printf("End of kernel is at %d\n", &_kernel_end);
  int stack_size = get_stack_size();
  printf("Stack size = %d\n", stack_size);
  init_stack(stack_size);
  for(int i=0; i<num_free_mem; ++i){
    // Push free memory addresses in blocks of 4kb into stack
    const int num_frames = free_mem_list[i].length / frame_size;
    printf("Num frames = %d\n", num_frames);
    for(int j=0; j<num_frames; ++j){
      push((char *)free_mem_list[i].start + j*frame_size);
    }
  }
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
    printf("Free block starts at %d\n", (int *)free_mem_list[i].start);
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

int* get_page(){
  return (int *)pop();
}

void free_page(int *page){
  push((int)page);
}

