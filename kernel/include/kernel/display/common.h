#include <stdint.h>

typedef struct
{
    void *framebuffer;
    uint16_t width;
    uint16_t height;
    uint16_t pitch;
} vbe_info_t;

/*
Recently drawn (x, y) pixel position.
Useful to select a starting position to draw something.
*/
typedef struct
{
    uint16_t x;
    uint16_t y;
} position_info_t;

typedef struct
{
    uint32_t fg;
    uint32_t bg;
} color_info_t;

extern vbe_info_t vbe_info;
extern position_info_t position;
extern color_info_t color_info;

void *ptr_to_y_pos();
uint32_t skip_line(const int n);