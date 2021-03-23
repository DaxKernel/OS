#include <kernel/mem/mem.h>
#include <kernel/mem/p_stack.h>
#include <kernel/mem/mmap.h>
#include <stdio.h>

extern char _kernel_end;
// Size of each page frame in KB
static const int frame_size = 4 * 1024;
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
      stack_size += entry->length_low;
}

void parse_available_mem(){
  mmap_iterate(remove_overlap);
  stack_size = stack_size / frame_size;
}

uint32_t* get_page(){
  return (uint32_t *)pop();
}

void free_page(uint32_t *page){
  push((uint32_t)page);
}