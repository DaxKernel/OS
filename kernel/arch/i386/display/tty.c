#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/panic.h>
#include <kernel/image.h>
#define SSFN_CONSOLEBITMAP_TRUECOLOR
#include <kernel/ssfn.h>

void test_draw()
{
    for (uint32_t *ptr = (uint32_t *)ssfn_dst.ptr, c = 0; c < 10; ++c, ++ptr)
        *ptr = 0xffff0000;
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
    ssfn_dst.y = ssfn_dst.y - ssfn_src->height;
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
    uint32_t *ptr = (uint32_t *)ssfn_current_line();
    // Go five rows of pixels down
    ptr = (uint32_t *)((char *)ptr + 5 * ssfn_dst.p);
    for (int i = 0; i < ssfn_dst.w; ++i, ++ptr)
    {
        *ptr = white;
    }
    ssfn_dst.y = ssfn_dst.y + ssfn_src->height;
}

void tty_print_with_color(const char *string, uint32_t color)
{
    const uint32_t current_color = ssfn_dst.fg;
    ssfn_dst.fg = color;
    tty_write_string(string);
    ssfn_dst.fg = current_color;
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
    ssfn_dst.x = (ssfn_dst.w - n_pixel) / 2;
    tty_write_string(string);
    tty_insert_char('\n');
}

void tty_print_header()
{
    uint32_t color = ssfn_dst.fg;
    ssfn_dst.fg = sunset_orange;
    tty_write_string_centered("DAX Operating System");
    tty_write_string_centered("2021 (c)");
    tty_print_seperator();
    ssfn_dst.fg = color;
}

void tty_initialize(multiboot_info_t *mbt)
{
    extern char _binary_unifont_sfn_start;
    ssfn_from_vesa(mbt, &_binary_unifont_sfn_start);
    screenfull_handler = &tty_push_text_upward;
    tty_print_success("VESA Graphics Driver", "OK");
    // fill_debug_text();
}

typedef struct
{
    uint16_t width;
    uint16_t height;
    void *pixels;
} image_t;

image_t new_image_t(uint16_t width, uint16_t height, void *image)
{
    image_t result;
    result.width = width;
    result.height = height;
    result.pixels = image;
    return result;
}

image_t get_image()
{
    void *image = nyan_cat_image();
    tga_header_t t = tga_header(image);
    return new_image_t(t.w, t.h, pixel_data(image));
}

void *get_start_position()
{
    return (char *)ssfn_current_line() + skip_line(5);
}

int draw_row_of_pixel(void *dest, void *pixel_ptr, const int pixels_per_row)
{
    memcpy(dest, pixel_ptr, sizeof(uint32_t) * pixels_per_row);
    return pixels_per_row;
}

void draw_row_by_row(image_t image)
{
    uint32_t *read = image.pixels;
    char *write = get_start_position();
    for (int i = 0; i < image.height; ++i)
    {
        read += draw_row_of_pixel(write, read, image.width);
        write += skip_line(1);
    }
}

void draw_image()
{
    image_t image = get_image();
    draw_row_by_row(image);
}