#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
#include "vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void tty_clear_row(size_t row) {
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        const size_t index = row * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void tty_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        tty_clear_row(y);
}

void tty_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    terminal_buffer = VGA_MEMORY;
    tty_clear();
    tty_print_success("Terminal Status", "OK");
}

void tty_setcolor(uint8_t color) {
    terminal_color = color;
}

void tty_put_entry_at(unsigned char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

static inline void tty_newline() {
    ++terminal_row;
    terminal_column = 0;
}

void tty_position_for_next_char() {
    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;
        }
    }
}

void tty_insert_char(char c) {
    unsigned char uc = c;
    tty_put_entry_at(uc, terminal_color, terminal_column, terminal_row);
    tty_position_for_next_char();
}

void tty_push_text_upward() {
    size_t y;
    for (y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
        }
    }
    tty_clear_row(y);
}

void tty_backspace(){
    if(terminal_column == 0){
        terminal_column = VGA_WIDTH - 1;
        terminal_row--;
    }
    else terminal_column--;

    tty_put_entry_at(' ',terminal_color, terminal_column, terminal_row);
}

void tty_put_char(char c) {
    if (c == '\n')
        tty_newline();
    else if(c == '\b')
        tty_backspace();
    else
        tty_insert_char(c);
    if (terminal_row >= VGA_HEIGHT) {
        // At this point screen is full of text
        // Push rows upward
        tty_push_text_upward();
        terminal_row--;
    }
}

void tty_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        tty_put_char(data[i]);
}

void tty_write_string(const char *data) {
    tty_write(data, strlen(data));
}

void tty_write_string_centered(const char *string) {
    const size_t str_length = strlen(string);
    if (str_length < VGA_WIDTH) {
        // Center text if and only if it is possible to do so.
        // Otherwise merely print the text
        const int skip = (VGA_WIDTH - str_length) / 2;
        terminal_column = skip;
    }
    tty_write_string(string);
    tty_newline();
}

void tty_print_horizontal_rule(const char symbol) {
    if (terminal_column != 0)
        tty_newline();
    for (size_t i = 0; i < VGA_WIDTH; ++i) {
        tty_put_char(symbol);
    }
    tty_newline();
}

void tty_print_success(const char *string, const char *success_string) {
    uint8_t c_color = terminal_color;
    tty_write_string(string);
    tty_write_string(" - [");
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    tty_write_string(success_string);
    terminal_color = c_color;
    tty_write_string("]\n");
}