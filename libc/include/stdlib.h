#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C"
{
#endif

    char *itoa(int, char *);

    void abort(void) __attribute__((__noreturn__));

#ifdef __cplusplus
}
#endif

#endif
