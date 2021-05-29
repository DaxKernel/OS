#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/display/tty.h>
#include <kernel/panic.h>
#include <kernel/display/image.h>
#include <kernel/display/text.h>
#include <kernel/display/common.h>

vbe_info_t vbe_info;
color_info_t color_info;
position_info_t position;

void test_draw()
{
    for (uint32_t *ptr = (uint32_t *)vbe_info.framebuffer, c = 0; c < 10; ++c, ++ptr)
        *ptr = 0xffff0000;
}

void tty_clear()
{
    memset(vbe_info.framebuffer, 0, vbe_info.height * vbe_info.width * sizeof(uint32_t));
}

void fill_debug_text()
{
    for (int i = 0; i < ssfn_qty.tl - 11; ++i)
    {
        tty_write_string("For a GUI, you'll probably want to display icons.\n");
    }
}

void tty_insert_char(char c)
{
    ssfn_putc(c);
}

void tty_push_text_upward()
{
    ssfn_push_rows_upwards();
    position.y = position.y - ssfn_src->height;
    ssfn_clr_line();
}

void tty_write(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        tty_insert_char(data[i]);
}

void tty_write_string(const char *data)
{
    tty_write(data, strlen(data));
}

void tty_print_seperator()
{
    uint32_t *ptr = (uint32_t *)ptr_to_y_pos();
    // Go five rows of pixels down
    ptr = (uint32_t *)((char *)ptr + 5 * vbe_info.pitch);
    for (int i = 0; i < vbe_info.width; ++i, ++ptr)
    {
        *ptr = white;
    }
    position.y = position.y + ssfn_src->height;
}

void tty_print_with_color(const char *string, uint32_t color)
{
    const uint32_t current_color = color_info.fg;
    color_info.fg = color;
    tty_write_string(string);
    color_info.fg = current_color;
}

void tty_print_rainbow(const char *string)
{
    uint32_t colors[] = {0xffff0000, 0xffffa500, 0xffffff00, 0xff008000, 0xff0000ff, 0xff4b0082, 0xffee82ee};
    const int color_size = 7;
    for (size_t i = 0; i < strlen(string); ++i)
    {
        char s[2];
        s[0] = string[i];
        s[1] = '\0';
        tty_print_with_color(s, colors[i % color_size]);
    }
}

void tty_print_success(const char *string, const char *success_string)
{
    tty_write_string(string);
    tty_write_string(" - [");
    tty_print_with_color(success_string, lime);
    tty_write_string("]\n");
}

void tty_write_string_centered(const char *string)
{
    const int len = strlen(string);
    const int n_pixel = ssfn_qty.ppc * len;
    position.x = (vbe_info.width - n_pixel) / 2;
    tty_write_string(string);
    tty_insert_char('\n');
}

void tty_print_header()
{
    uint32_t color = color_info.fg;
    color_info.fg = sunset_orange;
    tty_write_string_centered("DAX Operating System");
    tty_write_string_centered("2021 (c)");
    tty_print_seperator();
    color_info.fg = color;
}

void set_vbe_info(multiboot_info_t *mbt)
{
    vbe_info.framebuffer = (void *)(intptr_t)mbt->framebuffer_addr;
    vbe_info.height = mbt->framebuffer_height;
    vbe_info.width = mbt->framebuffer_width;
    vbe_info.pitch = mbt->framebuffer_pitch;
}

void set_color(uint32_t bg, uint32_t fg)
{
    color_info.bg = bg;
    color_info.fg = fg;
}

void set_cursor(uint16_t x, uint16_t y)
{
    position.x = x;
    position.y = y;
}

void tty_initialize(multiboot_info_t *mbt)
{
    extern char _binary_unifont_sfn_start;
    set_vbe_info(mbt);
    set_color(black, white);
    set_cursor(0, 0);
    ssfn_load_font(&_binary_unifont_sfn_start);
    screenfull_handler = &tty_push_text_upward;
    tty_print_success("VESA Graphics Driver", "OK");
    // fill_debug_text();
}
