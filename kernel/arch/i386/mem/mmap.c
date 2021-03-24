#include <kernel/mem/mmap.h>
#include <stdint.h>
#include <stdbool.h>

static const uint32_t MB = 1024 * 1024;
multiboot_info_t* mbt;

static inline mmap_entry_t* get_entry(){
  return (mmap_entry_t *)mbt->mmap_addr;
}

static inline bool not_entry_end(mmap_entry_t* entry){
  return entry < (mmap_entry_t*)((char *)mbt->mmap_addr + mbt->mmap_length);
}

static inline mmap_entry_t* next_entry(mmap_entry_t* entry){
  return (mmap_entry_t*) ((uintptr_t) entry + entry->size + sizeof(entry->size));
}

void mmap_iterate(void (*func)(mmap_entry_t*)){
  for(mmap_entry_t* entry = get_entry(); not_entry_end(entry); entry = next_entry(entry))
      {
        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE && entry->base_addr_low >= MB)
          (*func)(entry);
      }
}

void set_multiboot_info_t(multiboot_info_t* m){
  mbt = m;
}
