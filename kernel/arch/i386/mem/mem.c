#include <kernel/mem/mem.h>
#include <kernel/mem/p_stack.h>
#include <kernel/mem/mmap.h>
#include <kernel/mem/f_size.h>
#include <stdio.h>
#include <stddef.h>

extern char _kernel_end;
static int stack_size = -1;

void push_frames(mmap_entry_t *entry){
   for(int j=0; j<stack_size; ++j)
        push((uint32_t)((char *)entry->base_addr_low + j*frame_size));
}

void allocate_buffer(){
  init_stack(stack_size);
  mmap_iterate(push_frames);
}

void remove_overlap(mmap_entry_t* entry){
     if(entry->base_addr_low < (uintptr_t)&_kernel_end){
        entry->length_low -= (uint32_t)((char *)&_kernel_end - (char *)entry->base_addr_low);
        entry->base_addr_low = (uint32_t)&_kernel_end;     
      }
      stack_size += entry->length_low;
}

void parse_available_mem(){
  /*
  Let T be the total number of bytes of free memory after adjusting for the kernel
  Let m the number of page frames such that when the free memory is divided into m pages,
  it leaves exactly enough space for the stack.
  Notice that if you have m pages, then you need a stack of size m to keep track of it.
  Also remember that each element of stack needs sizeof(int) bytes.
  Then we have,
  (frame_size * m) + (m * sizeof(int)) = T
  => 4096m + 4m = T
  => 4100m = T
  => m = 4100/T
  */
  const int sep = 4100;
  mmap_iterate(remove_overlap);
  stack_size /= sep;
}

uint32_t* get_page(){
  int32_t mem = pop();
  return mem == -1? NULL : (uint32_t *)mem;
}

void free_page(uint32_t *page){
  push((uint32_t)page);
}

size_t get_num_free_pages(){
  return get_stack_count();
}