#include <kernel/display/image.h>
#include <kernel/display/tga.h>

void *nyan_cat_image()
{
    extern void *_binary_nyan_tga_start;
    return &_binary_nyan_tga_start;
}

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
