#include <kernel/mem/mmap.h>
#include <stdint.h>

static const uint32_t MB = 1024 * 1024;
multiboot_info_t* mbt;

void mmap_iterate(void (*func)(mmap_entry_t*)){
  mmap_entry_t* entry = (mmap_entry_t *)mbt->mmap_addr;
  while(entry < (mmap_entry_t*)((char *)mbt->mmap_addr + mbt->mmap_length)) {
    if(entry->type == MULTIBOOT_MEMORY_AVAILABLE && entry->base_addr_low >= MB)
      (*func)(entry);
    entry = (mmap_entry_t*) ((uintptr_t) entry + entry->size + sizeof(entry->size));
  }
}

void set_multiboot_info_t(multiboot_info_t* m){
  mbt = m;
}
