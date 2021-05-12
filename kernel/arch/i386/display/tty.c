#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <kernel/tty.h>
#include <kernel/panic.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <kernel/ssfn.h>

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
    extern char _binary_unifont_sfn_start;
    if (!verify_mbt_framebuffer(mbt))
        k_panic("Framebuffer info not set by GRUB!");
    ssfn_src = (ssfn_font_t *)&_binary_unifont_sfn_start;
    ssfn_dst.ptr = (uint8_t *)(intptr_t)mbt->framebuffer_addr;
    ssfn_dst.p = mbt->framebuffer_pitch;
    ssfn_dst.w = mbt->framebuffer_width;
    ssfn_dst.fg = 0xFFFFFFFF;
    ssfn_dst.bg = 0xFF000000;
    ssfn_dst.x = 0;
    ssfn_dst.y = 0;
    tty_print_success("VESA Graphics Driver", "OK");
}

void tty_insert_char(char c)
{
    ssfn_putc(c);
}

void tty_push_text_upward()
{
}

void tty_backspace()
{
    const uint8_t char_width = ssfn_src->width / 2;
    ssfn_dst.x -= char_width;
    ssfn_putc(' ');
    ssfn_dst.x -= char_width;
}

void tty_put_char(char c)
{
    if (c == '\n')
        ssfn_putc('\n');
    else if (c == '\b')
        tty_backspace();
    else
        tty_insert_char(c);
    /*     if (terminal_row >= VGA_HEIGHT)
    {
        // At this point screen is full of text
        // Push rows upward
        tty_push_text_upward();
        terminal_row--;
    } */
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

void tty_print_horizontal_rule(/* const char symbol */)
{
    /*     for (size_t i = 0; i < VGA_WIDTH; ++i)
    {
        tty_put_char(symbol);
    }
    tty_newline(); */
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