#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void tty_initialize(void);
void tty_put_char(char c);
void tty_write(const char *data, size_t size);
void tty_write_string(const char *data);
void tty_write_string_centered(const char *string);
void tty_print_horizontal_rule(const char symbol);
void tty_setcolor(uint8_t color);
void tty_print_success(const char *string, const char *success_string);
#endif
