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

#include <kernel/display/text.h>
#include <kernel/display/common.h>
#include <stdlib.h>
#include <string.h>

/* error codes */
#define SSFN_OK 0            /* success */
#define SSFN_ERR_INVINP -4   /* invalid input */
#define SSFN_ERR_NOGLYPH -7  /* glyph (or kerning info) not found */

/**
 * public variables to configure
 */
ssfn_font_t *ssfn_src; /* font buffer with an inflated bitmap font */
ssfn_buf_t ssfn_dst;   /* destination frame buffer */
ssfn_qty_t ssfn_qty;

/**
 * Pointer to function that must be called when screen is full.
 */
void (*screenfull_handler)();

/**
 * Pointer to pen position
 */
void *ssfn_get_pos()
{
    unsigned char *pos = (uint8_t *)vbe_info.framebuffer + position.y * vbe_info.pitch + position.x * sizeof(uint32_t);
    return (void *)pos;
}

/**
 * Pointer to start of current line
 */
void *ssfn_current_line()
{
    void *result = (char *)vbe_info.framebuffer + vbe_info.pitch * position.y;
    return result;
}

uint32_t skip_line(const int n)
{
    return vbe_info.pitch * n;
}

/**
 * Clear line pointed by position.y.
 * Puts x-cursor at start of cleared line.
 */
void ssfn_clr_line()
{
    position.x = 0;
    char *start = (char *)ssfn_current_line();
    const int bytes_per_line = vbe_info.pitch * ssfn_src->height;
    memset(start, 0, bytes_per_line);
}

/**
 * Erase last character and move x-cursor one position back
 */
void ssfn_backspace()
{
    position.x = position.x - ssfn_qty.ppc;
    uint32_t *start = (uint32_t *)(vbe_info.framebuffer + position.y * vbe_info.pitch + position.x * 4);
    uint32_t *ptr = start;
    for (uint8_t i = 0; i < ssfn_src->height; ++i)
    {
        memset(ptr, 0, ssfn_qty.ppc * 4);
        ptr = start + (i + 1) * (vbe_info.pitch / 4);
    }
}

void calculate_ssfn_qty()
{
    ssfn_qty.ppc = ssfn_src->width / 2;
    ssfn_qty.tl = vbe_info.height / ssfn_src->height;
}

/**
 * Initialize ssfn global configuration from multiboot
 * and font.
 */
void ssfn_from_vesa(multiboot_info_t *mbt, void *font)
{
    ssfn_src = (ssfn_font_t *)font;
    vbe_info.framebuffer = (uint8_t *)(intptr_t)mbt->framebuffer_addr;
    vbe_info.pitch = mbt->framebuffer_pitch;
    vbe_info.width = mbt->framebuffer_width;
    vbe_info.height = mbt->framebuffer_height;
    ssfn_dst.fg = 0xFFFFFFFF;
    ssfn_dst.bg = 0xFF000000;
    position.x = 0;
    position.y = 0;
    calculate_ssfn_qty();
}

/**
 * If screen is full of text, push text up one row
 */
void handle_screenful()
{
    const int skip = vbe_info.pitch * vbe_info.height;
    const unsigned char *end = (uint8_t *)vbe_info.framebuffer + skip;
    const unsigned char *pos = ssfn_get_pos();
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
    unsigned char *second_line = vbe_info.framebuffer + vbe_info.pitch * ssfn_src->height;
    const int n = vbe_info.pitch * (vbe_info.height - ssfn_src->height);
    memmove(vbe_info.framebuffer, second_line, n);
}

/**
 * Minimal OS kernel console renderer
 *
 * @param unicode character
 * @return error code
 */
int ssfn_putc(uint32_t unicode)
{
#define SSFN_PIXEL uint32_t
    register SSFN_PIXEL *o, *p;
    register uint8_t *ptr, *chr = NULL, *frg;
    register int i, j, k, l, m, y = 0, w, s = vbe_info.pitch / sizeof(SSFN_PIXEL);

    if (!ssfn_src || ssfn_src->magic[0] != 'S' || ssfn_src->magic[1] != 'F' || ssfn_src->magic[2] != 'N' ||
        ssfn_src->magic[3] != '2' || !vbe_info.framebuffer || !vbe_info.pitch)
        return SSFN_ERR_INVINP;
    if (unicode == '\r' || unicode == '\n')
    {
        position.x = 0;
        if (unicode == '\n')
        {
            position.y += ssfn_src->height;
            handle_screenful();
        }
        return SSFN_OK;
    }
    else if (unicode == '\b')
    {
        ssfn_backspace();
        return SSFN_OK;
    }
    w = vbe_info.width;
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
    o = (SSFN_PIXEL *)((uint8_t *)vbe_info.framebuffer + position.y * vbe_info.pitch + position.x * sizeof(SSFN_PIXEL));
    for (i = 0; i < chr[1]; i++, ptr += chr[0] & 0x40 ? 6 : 5)
    {
        if (ptr[0] == 255 && ptr[1] == 255)
            continue;
        frg = (uint8_t *)ssfn_src + (chr[0] & 0x40 ? ((ptr[5] << 24) | (ptr[4] << 16) | (ptr[3] << 8) | ptr[2]) : ((ptr[4] << 16) | (ptr[3] << 8) | ptr[2]));
        if ((frg[0] & 0xE0) != 0x80)
            continue;
        if (ssfn_dst.bg)
        {
            for (; y < ptr[1] && (!vbe_info.height || position.y + y < vbe_info.height); y++, o += s)
            {
                for (p = o, j = 0; j < chr[2] && (!w || position.x + j < w); j++, p++)
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
        for (m = 1; j && (!vbe_info.height || position.y + y < vbe_info.height); j--, y++, o += s)
            for (p = o, l = 0; l < k; l++, p++, m <<= 1)
            {
                if (m > 0x80)
                {
                    frg++;
                    m = 1;
                }
                if (position.x + l >= 0 && (!w || position.x + l < w))
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
        for (; y < chr[3] && (!vbe_info.height || position.y + y < vbe_info.height); y++, o += s)
        {
            for (p = o, j = 0; j < chr[2] && (!w || position.x + j < w); j++, p++)
                *p = ssfn_dst.bg;
        }
    position.x += chr[4];
    position.y += chr[5];
    return SSFN_OK;
}
