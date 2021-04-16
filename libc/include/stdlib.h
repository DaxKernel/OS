#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>
#include <kernel/mem/liballoc.h>

#ifdef __cplusplus
extern "C"
{
#endif

    char *itoa(int, char *);
    char *uitoa(unsigned int, char *);

    void abort(void) __attribute__((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif
