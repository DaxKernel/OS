#include <stdlib.h>
#include <kernel/display/image.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

tga_header_t tga_header(void *image)
{
    tga_header_t t;
    memcpy(&t, image, sizeof(tga_header_t));
    return t;
}

void *pixel_data(void *image)
{
    return (char *)image + sizeof(tga_header_t);
}

void *nyan_cat_image()
{
    extern char _binary_nyan_tga_start;
    return &_binary_nyan_tga_start;
}
