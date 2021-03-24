#ifndef _LIBALLOC_H
#define _LIBALLOC_H

#include <stddef.h>
#include <stdint.h>

// Hooks needed; refer to hooks.c file for more information.
extern int liballoc_lock();
extern int liballoc_unlock();
extern void* liballoc_alloc(size_t);
extern int liballoc_free(void*,size_t);

// Our functions
extern void    *malloc(size_t);
extern void    *realloc(void *, size_t);
extern void    *calloc(size_t, size_t);
extern void     free(void *);

#endif


