#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char *data, size_t size);
void terminal_writestring(const char *data);
void terminal_write_string_centered(const char *string);
void terminal_print_horizontal_rule(const char symbol);
void terminal_setcolor(uint8_t color);
void terminal_print_success(const char *string);
#endif
