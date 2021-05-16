#ifndef IMAGE_H
#define IMAGE_H
typedef struct
{
    unsigned char magic1;             // must be zero
    unsigned char colormap;           // must be zero
    unsigned char encoding;           // must be 2
    unsigned short cmaporig, cmaplen; // must be zero
    unsigned char cmapent;            // must be zero
    unsigned short x;                 // must be zero
    unsigned short y;                 // image's height
    unsigned short w;                 // image's width
    unsigned short h;                 // image's height
    unsigned char bpp;                // must be 32
    unsigned char pixeltype;          // must be 40
} __attribute__((packed)) tga_header_t;

tga_header_t tga_header(void *image);

void *pixel_data(void *image);

void *nyan_cat_image();

#endif