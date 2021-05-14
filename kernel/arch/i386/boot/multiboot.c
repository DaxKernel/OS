#include <kernel/multiboot_32.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>
#include <stdio.h>

inline void verify_multiboot_header(uint32_t magic){
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC){
        k_panic("Invalid Bootloader\n");
    }
}

inline void verify_multiboot_mmap(multiboot_info_t* mbt){
  if((mbt->flags & (1<<6)) == 0){
      k_panic("Could not get memory map from Multiboot.\n");
  }
}

inline void verify_multiboot_framebuffer(multiboot_info_t *mbt)
{
    const uint32_t flags = mbt->flags;
    const uint32_t bitmask = 1 << 12;
    if (!(flags & bitmask))
    {
        k_panic("Multiboot framebuffer not valid!");
    }
}

inline void verify_multiboot(uint32_t magic, multiboot_info_t* mbt){
    verify_multiboot_header(magic);
    verify_multiboot_mmap(mbt);
    verify_multiboot_framebuffer(mbt);
}