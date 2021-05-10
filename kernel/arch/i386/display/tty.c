#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include "vga.h"

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#define _STDINT_H
#include <kernel/ssfn.h>

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t *terminal_buffer;

void tty_clear_row(size_t row)
{
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        const size_t index = row * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

void tty_clear()
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        tty_clear_row(y);
}

bool verify_mbt_framebuffer(multiboot_info_t *mbt)
{
    const uint32_t flags = mbt->flags;
    const uint32_t bitmask = 1 << 12;
    if (flags & bitmask)
        return true;
    return false;
}

void tty_initialize(multiboot_info_t *mbt)
{
    extern unsigned char _binary_unifont_sfn_start;
    if (!verify_mbt_framebuffer(mbt))
        k_panic("Framebuffer info not set by GRUB!");
    ssfn_src = &_binary_unifont_sfn_start;
    ssfn_dst.ptr = (uint8_t *)mbt->framebuffer_addr;
    ssfn_dst.p = mbt->framebuffer_pitch;
    ssfn_dst.w = mbt->framebuffer_width;
    ssfn_dst.fg = 0xFFFFFFFF;
    ssfn_dst.bg = 0xFF000000;
    ssfn_dst.x = 0;
    ssfn_dst.y = 0;
}

static inline void tty_newline()
{
    ssfn_putc('\n');
}

void tty_insert_char(char c)
{
    ssfn_putc(c);
}

void tty_push_text_upward()
{
    size_t y;
    for (y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
        }
    }
    tty_clear_row(y);
}

void tty_backspace()
{
    ssfn_dst.x -= x_skip;
    ssfn_putc(' ');
    ssfn_dst.x -= x_skip;
}

void tty_put_char(char c)
{
    if (c == '\n')
        tty_newline();
    else if (c == '\b')
        tty_backspace();
    else
        tty_insert_char(c);
    if (terminal_row >= VGA_HEIGHT)
    {
        // At this point screen is full of text
        // Push rows upward
        tty_push_text_upward();
        terminal_row--;
    }
}

void tty_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        tty_put_char(data[i]);
}

void tty_write_string(const char *data)
{
    tty_write(data, strlen(data));
}

void tty_print_horizontal_rule(const char symbol)
{
    if (terminal_column != 0)
        tty_newline();
    for (size_t i = 0; i < VGA_WIDTH; ++i)
    {
        tty_put_char(symbol);
    }
    tty_newline();
}

void tty_print_success(const char *string, const char *success_string)
{
    uint32_t c_color = ssfn_dst.fg;
    tty_write_string(string);
    tty_write_string(" - [");
    uint32_t s_color = 0xFF00FF00;
    ssfn_dst.fg = s_color;
    tty_write_string(success_string);
    ssfn_dst.fg = c_color;
    tty_write_string("]\n");
}