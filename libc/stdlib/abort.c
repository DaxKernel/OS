#include <kernel/panic.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void abort(void)
{
	k_panic("kernel: panic: abort()\n");
	while (1) { }
	__builtin_unreachable();
}
