#include <kernel/display/tga.h>
#include <string.h>

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