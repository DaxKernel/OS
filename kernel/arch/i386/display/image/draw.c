#include <kernel/display/draw.h>
#include <stdint.h>
#include <string.h>
#include <kernel/display/common.h>

void *get_start_position()
{
    return (char *)ptr_to_y_pos() + skip_line(5);
}

int copy_row_of_pixel(void *dest, void *pixel_ptr, const int pixels_per_row)
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
        read += copy_row_of_pixel(write, read, image.width);
        write += skip_line(1);
    }
}

void draw_image()
{
    image_t image = get_image();
    draw_row_by_row(image);
    position.y += image.height;
}