#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	terminal_initialize();
	printf("DAX Operating System\nBuild 1.0 2020 (c)");
}
