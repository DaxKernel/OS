#include <stdint.h>
#include <kernel/multiboot.h>

/* destination frame buffer context */
typedef struct
{
    uint8_t *ptr; /* pointer to the buffer */
    int16_t w;    /* Width of framebuffer (positive: ARGB, negative: ABGR pixels) */
    int16_t h;    /* Height of framebuffer */
    uint16_t p;   /* pitch, bytes per line */
    int16_t x;    /* cursor x */
    int16_t y;    /* cursor y */
    uint32_t fg;  /* foreground color */
    uint32_t bg;  /* background color */
} ssfn_buf_t;

/**
 * Precalculated helpful quantities 
 */
typedef struct
{
    // Pixels per character
    uint8_t ppc;
    // Total number of lines
    uint8_t tl;
} ssfn_qty_t;

typedef struct
{
    uint8_t magic[4];         /* SSFN magic bytes */
    uint32_t size;            /* total size in bytes */
    uint8_t type;             /* font family and style */
    uint8_t features;         /* format features and revision */
    uint8_t width;            /* overall width of the font */
    uint8_t height;           /* overall height of the font */
    uint8_t baseline;         /* horizontal baseline in grid pixels */
    uint8_t underline;        /* position of under line in grid pixels */
    uint16_t fragments_offs;  /* offset of fragments table */
    uint32_t characters_offs; /* characters table offset */
    uint32_t ligature_offs;   /* ligatures table offset */
    uint32_t kerning_offs;    /* kerning table offset */
    uint32_t cmap_offs;       /* color map offset */
} __attribute__((packed)) ssfn_font_t;

enum color
{
    red = 0xFFFF0000,
    green = 0xFF00FF00,
    blue = 0xFF0000FF,
    lime = 0xFFBFFF00,
    white = 0xFFFFFFFF,
    black = 0xFF000000,
    sunset_orange = 0xFFFA534D
};

/**
 * Pointer to function that must be called when screen is full.
 */
extern void (*screenfull_handler)();

extern ssfn_font_t *ssfn_src; /* font buffer */
extern ssfn_buf_t ssfn_dst;   /* destination frame buffer */
extern ssfn_qty_t ssfn_qty;

int ssfn_putc(uint32_t unicode);
void *ssfn_get_pos();
void *ssfn_current_line();
uint32_t skip_line(const int n);
void ssfn_clr_line();
void ssfn_backspace();
void ssfn_from_vesa(multiboot_info_t *mbt, void *font);
void ssfn_push_rows_upwards();