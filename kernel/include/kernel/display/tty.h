#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>
#include <kernel/multiboot.h>

void tty_initialize(multiboot_info_t *mbt);
void tty_print_header();
void tty_insert_char(char c);
void tty_write(const char *data, size_t size);
void tty_write_string(const char *data);
void tty_print_horizontal_rule();
void tty_write_string_centered(const char *string);
void tty_setcolor(uint8_t color);
void tty_print_success(const char *string, const char *success_string);
void tty_clear();
void tty_print_seperator();
void tty_print_with_color(const char *string, uint32_t color);
void tty_print_rainbow(const char *string);
void draw_image();
#endif
