#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/panic.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <kernel/ssfn.h>

uint16_t curr_line_count = 0;

void test_draw()
{
    for (uint32_t *ptr = ssfn_dst.ptr, c = 0; c < 10; ++c, ++ptr)
        *ptr = 0xffff0000;
}

void fill_debug_text()
{
    for (int i = 0; i < ssfn_qty.tl - 11; ++i)
    {
        tty_write_string("For a GUI, you'll probably want to display icons.\n");
    }
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
    extern char _binary_unifont_sfn_start;
    if (!verify_mbt_framebuffer(mbt))
        k_panic("Framebuffer info not set by GRUB!");
    ssfn_from_vesa(mbt, &_binary_unifont_sfn_start);
    tty_print_success("VESA Graphics Driver", "OK");
    fill_debug_text();
}

void tty_insert_char(char c)
{
    ssfn_putc(c);
}

void tty_push_text_upward()
{
    char *src = ssfn_dst.ptr + ssfn_dst.p * ssfn_src->height;
    char *end = (uint32_t *)ssfn_dst.ptr + (ssfn_dst.w * ssfn_dst.h);
    memmove(ssfn_dst.ptr, src, end - src);
    curr_line_count--;
    ssfn_dst.y -= ssfn_src->height;
    ssfn_dst.x = 0;
    char *start = (uint8_t *)ssfn_dst.ptr + ssfn_dst.y * ssfn_dst.p;
    const int ppl = ssfn_dst.p * ssfn_src->height;
    memset(start, 0, ppl);
}

void tty_backspace()
{
    ssfn_backspace();
}

void tty_put_char(char c)
{
    if (c == '\n')
    {
        curr_line_count++;
        ssfn_putc('\n');
    }
    else if (c == '\b')
        tty_backspace();
    else
        tty_insert_char(c);
    if (curr_line_count == ssfn_qty.tl)
    {
        tty_push_text_upward();
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