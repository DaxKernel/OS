/*
 * ssfn.h
 * https://gitlab.com/bztsrc/scalable-font2
 *
 * Copyright (C) 2020 bzt (bztsrc@gitlab)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * @brief Scalable Screen Font renderers
 *
 */

#ifndef _SSFN_H_
#define _SSFN_H_

#ifndef _STDINT_H
#include <stdint.h>
#endif

#define SSFN_VERSION 0x0200
/***** file format *****/

/* magic bytes */
#define SSFN_MAGIC "SFN2"
#define SSFN_COLLECTION "SFNC"
#define SSFN_ENDMAGIC "2NFS"

/* ligatures area */
#define SSFN_LIG_FIRST 0xF000
#define SSFN_LIG_LAST 0xF8FF

/* main SSFN header, 32 bytes */
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

/* error codes */
#define SSFN_OK 0            /* success */
#define SSFN_ERR_ALLOC -1    /* allocation error */
#define SSFN_ERR_BADFILE -2  /* bad SSFN file format */
#define SSFN_ERR_NOFACE -3   /* no font face selected */
#define SSFN_ERR_INVINP -4   /* invalid input */
#define SSFN_ERR_BADSTYLE -5 /* bad style */
#define SSFN_ERR_BADSIZE -6  /* bad size */
#define SSFN_ERR_NOGLYPH -7  /* glyph (or kerning info) not found */

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

/***** API function protoypes *****/

uint32_t ssfn_utf8(char **str); /* decode UTF-8 sequence */

/* simple renderer */
extern ssfn_font_t *ssfn_src;    /* font buffer */
extern ssfn_buf_t ssfn_dst;      /* destination frame buffer */
int ssfn_putc(uint32_t unicode); /* render console bitmap font */

/***** renderer implementations *****/

/*** these go for both renderers ***/
#if (defined(SSFN_IMPLEMENTATION) || defined(SSFN_CONSOLEBITMAP_PALETTE) ||           \
     defined(SSFN_CONSOLEBITMAP_HICOLOR) || defined(SSFN_CONSOLEBITMAP_TRUECOLOR)) && \
    !defined(SSFN_COMMON)
#define SSFN_COMMON

/**
 * Error code strings
 */
const char *ssfn_errstr[] = {"",
                             "Memory allocation error",
                             "Bad file format",
                             "No font face found",
                             "Invalid input value",
                             "Invalid style",
                             "Invalid size",
                             "Glyph not found"};

/**
 * Decode an UTF-8 multibyte, advance string pointer and return UNICODE. Watch out, no input checks
 *
 * @param **s pointer to an UTF-8 string pointer
 * @return unicode, and *s moved to next multibyte sequence
 */
uint32_t ssfn_utf8(char **s)
{
    uint32_t c = **s;

    if ((**s & 128) != 0)
    {
        if (!(**s & 32))
        {
            c = ((**s & 0x1F) << 6) | (*(*s + 1) & 0x3F);
            *s += 1;
        }
        else if (!(**s & 16))
        {
            c = ((**s & 0xF) << 12) | ((*(*s + 1) & 0x3F) << 6) | (*(*s + 2) & 0x3F);
            *s += 2;
        }
        else if (!(**s & 8))
        {
            c = ((**s & 0x7) << 18) | ((*(*s + 1) & 0x3F) << 12) | ((*(*s + 2) & 0x3F) << 6) | (*(*s + 3) & 0x3F);
            *s += 3;
        }
        else
            c = 0;
    }
    (*s)++;
    return c;
}
#endif

#if defined(SSFN_CONSOLEBITMAP_PALETTE) || defined(SSFN_CONSOLEBITMAP_HICOLOR) || defined(SSFN_CONSOLEBITMAP_TRUECOLOR)
/*** special console bitmap font renderer (ca. 1.5k, no dependencies, no memory allocation and no error checking) ***/

/**
 * public variables to configure
 */
ssfn_font_t *ssfn_src; /* font buffer with an inflated bitmap font */
ssfn_buf_t ssfn_dst;   /* destination frame buffer */

/**
 * Precalculated helpful quantities 
 */
struct
{
    // Pixels per character
    uint8_t ppc;
    // Total number of lines
    uint8_t tl;
} ssfn_qty;

/**
 * Pointer to function that must be called when screen is full.
 */
void (*screenfull_handler)();

/**
 * Clear line pointed by ssfn_dst.y.
 * Puts x-cursor at start of cleared line.
 */
void ssfn_clr_line()
{
    ssfn_dst.x = 0;
    char *start = (uint8_t *)ssfn_dst.ptr + ssfn_dst.y * ssfn_dst.p;
    const int bytes_per_line = ssfn_dst.p * ssfn_src->height;
    memset(start, 0, bytes_per_line);
}

/**
 * Erase last character and move x-cursor one position back
 */
void ssfn_backspace()
{
    ssfn_dst.x = ssfn_dst.x - ssfn_qty.ppc;
    uint32_t *start = ssfn_dst.ptr + ssfn_dst.y * ssfn_dst.p + ssfn_dst.x * 4;
    uint32_t *ptr = start;
    for (uint8_t i = 0; i < ssfn_src->height; ++i)
    {
        memset(ptr, 0, ssfn_qty.ppc * 4);
        ptr = start + (i + 1) * (ssfn_dst.p / 4);
    }
}

void calculate_ssfn_qty()
{
    ssfn_qty.ppc = ssfn_src->width / 2;
    ssfn_qty.tl = ssfn_dst.h / ssfn_src->height;
}

/**
 * Initialize ssfn global configuration from multiboot
 * and font.
 */
void ssfn_from_vesa(multiboot_info_t *mbt, void *font)
{
    ssfn_src = (ssfn_font_t *)font;
    ssfn_dst.ptr = (uint8_t *)(intptr_t)mbt->framebuffer_addr;
    ssfn_dst.p = mbt->framebuffer_pitch;
    ssfn_dst.w = mbt->framebuffer_width;
    ssfn_dst.h = mbt->framebuffer_height;
    ssfn_dst.fg = 0xFFFFFFFF;
    ssfn_dst.bg = 0xFF000000;
    ssfn_dst.x = 0;
    ssfn_dst.y = 0;
    calculate_ssfn_qty();
}

/**
 * Pointer to pen position
 */
void *ssfn_get_pos()
{
    char *pos = (uint8_t *)ssfn_dst.ptr + ssfn_dst.y * ssfn_dst.p + ssfn_dst.x * sizeof(uint32_t);
    return (void *)pos;
}

/**
 * If screen is full of text, push text up one row
 */
void handle_screenful()
{
    const int skip = ssfn_dst.p * ssfn_dst.h;
    const char *end = (uint8_t *)ssfn_dst.ptr + skip;
    const char *pos = ssfn_get_pos();
    if (pos >= end)
    {
        (*screenfull_handler)();
    }
}

/**
 * Push text-rows up once starting from second row of text
 */
void ssfn_push_rows_upwards()
{
    char *second_line = ssfn_dst.ptr + ssfn_dst.p * ssfn_src->height;
    const int n = ssfn_dst.p * (ssfn_dst.h - ssfn_src->height);
    memmove(ssfn_dst.ptr, second_line, n);
}

/**
 * Minimal OS kernel console renderer
 *
 * @param unicode character
 * @return error code
 */
int ssfn_putc(uint32_t unicode)
{
#ifdef SSFN_CONSOLEBITMAP_PALETTE
#define SSFN_PIXEL uint8_t
#else
#ifdef SSFN_CONSOLEBITMAP_HICOLOR
#define SSFN_PIXEL uint16_t
#else
#define SSFN_PIXEL uint32_t
#endif
#endif
    register SSFN_PIXEL *o, *p;
    register uint8_t *ptr, *chr = NULL, *frg;
    register int i, j, k, l, m, y = 0, w, s = ssfn_dst.p / sizeof(SSFN_PIXEL);

    if (!ssfn_src || ssfn_src->magic[0] != 'S' || ssfn_src->magic[1] != 'F' || ssfn_src->magic[2] != 'N' ||
        ssfn_src->magic[3] != '2' || !ssfn_dst.ptr || !ssfn_dst.p)
        return SSFN_ERR_INVINP;
    if (unicode == '\r' || unicode == '\n')
    {
        ssfn_dst.x = 0;
        if (unicode == '\n')
        {
            ssfn_dst.y += ssfn_src->height;
            handle_screenful();
        }
        return SSFN_OK;
    }
    else if (unicode == '\b')
    {
        ssfn_backspace();
        return SSFN_OK;
    }
    w = ssfn_dst.w < 0 ? -ssfn_dst.w : ssfn_dst.w;
    for (ptr = (uint8_t *)ssfn_src + ssfn_src->characters_offs, i = 0; i < 0x110000; i++)
    {
        if (ptr[0] == 0xFF)
        {
            i += 65535;
            ptr++;
        }
        else if ((ptr[0] & 0xC0) == 0xC0)
        {
            j = (((ptr[0] & 0x3F) << 8) | ptr[1]);
            i += j;
            ptr += 2;
        }
        else if ((ptr[0] & 0xC0) == 0x80)
        {
            j = (ptr[0] & 0x3F);
            i += j;
            ptr++;
        }
        else
        {
            if ((uint32_t)i == unicode)
            {
                chr = ptr;
                break;
            }
            ptr += 6 + ptr[1] * (ptr[0] & 0x40 ? 6 : 5);
        }
    }
    if (!chr)
        return SSFN_ERR_NOGLYPH;
    ptr = chr + 6;
    o = (SSFN_PIXEL *)((uint8_t *)ssfn_dst.ptr + ssfn_dst.y * ssfn_dst.p + ssfn_dst.x * sizeof(SSFN_PIXEL));
    for (i = 0; i < chr[1]; i++, ptr += chr[0] & 0x40 ? 6 : 5)
    {
        if (ptr[0] == 255 && ptr[1] == 255)
            continue;
        frg = (uint8_t *)ssfn_src + (chr[0] & 0x40 ? ((ptr[5] << 24) | (ptr[4] << 16) | (ptr[3] << 8) | ptr[2]) : ((ptr[4] << 16) | (ptr[3] << 8) | ptr[2]));
        if ((frg[0] & 0xE0) != 0x80)
            continue;
        if (ssfn_dst.bg)
        {
            for (; y < ptr[1] && (!ssfn_dst.h || ssfn_dst.y + y < ssfn_dst.h); y++, o += s)
            {
                for (p = o, j = 0; j < chr[2] && (!w || ssfn_dst.x + j < w); j++, p++)
                    *p = ssfn_dst.bg;
            }
        }
        else
        {
            o += (int)(ptr[1] - y) * s;
            y = ptr[1];
        }
        k = ((frg[0] & 0x1F) + 1) << 3;
        j = frg[1] + 1;
        frg += 2;
        for (m = 1; j && (!ssfn_dst.h || ssfn_dst.y + y < ssfn_dst.h); j--, y++, o += s)
            for (p = o, l = 0; l < k; l++, p++, m <<= 1)
            {
                if (m > 0x80)
                {
                    frg++;
                    m = 1;
                }
                if (ssfn_dst.x + l >= 0 && (!w || ssfn_dst.x + l < w))
                {
                    if (*frg & m)
                    {
                        *p = ssfn_dst.fg;
                    }
                    else if (ssfn_dst.bg)
                    {
                        *p = ssfn_dst.bg;
                    }
                }
            }
    }
    if (ssfn_dst.bg)
        for (; y < chr[3] && (!ssfn_dst.h || ssfn_dst.y + y < ssfn_dst.h); y++, o += s)
        {
            for (p = o, j = 0; j < chr[2] && (!w || ssfn_dst.x + j < w); j++, p++)
                *p = ssfn_dst.bg;
        }
    ssfn_dst.x += chr[4];
    ssfn_dst.y += chr[5];
    return SSFN_OK;
}

#endif /* SSFN_CONSOLEBITMAP */
#endif /* _SSFN_H_ */