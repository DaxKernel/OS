#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

typedef struct image_t
{
    uint16_t width;
    uint16_t height;
    void *pixels;
} image_t;

image_t get_image();

#endif