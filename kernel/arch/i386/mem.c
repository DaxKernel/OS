#include <kernel/mem.h>
#include <stdio.h>
extern char _kernel_end;

void allocate_buffer(){
  /* 
  Use this function to allocate memory at the end of the
  kernel
  This memory is to be used to track the free/used pages
  */
}