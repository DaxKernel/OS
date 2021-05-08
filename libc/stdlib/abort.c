#include <kernel/panic.h>
#include <stdio.h>
#include <stdlib.h>

#define __is_libk

__attribute__((__noreturn__))
void abort(void) {
#if defined(__is_libk)
	k_panic("kernel: panic: abort()\n");
#else
	printf("abort()\n");
#endif
	while (1) { }
	__builtin_unreachable();
}
