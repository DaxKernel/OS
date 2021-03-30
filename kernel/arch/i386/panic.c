#include <kernel/panic.h>
#include <stddef.h>
#include <stdint.h>

enum vga_color
{
    VGA_COLOR_RED = 4,
    VGA_COLOR_YELLOW = 14
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static size_t terminal_row = 0;
static size_t terminal_column = 0;
static uint8_t terminal_color = VGA_COLOR_YELLOW | VGA_COLOR_RED << 4;
static uint16_t *terminal_buffer = (uint16_t *)0xB8000;

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

static void kp_clear()
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

static void kp_put_entry_at(unsigned char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

static void kp_position_for_next_char()
{
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_row = 0;
        }
    }
}

static void kp_put_char(char c)
{
    if (c == '\n')
    {
        ++terminal_row;
        terminal_column = 0;
    }
    else
    {
        kp_put_entry_at(c, terminal_color, terminal_column, terminal_row);
        kp_position_for_next_char();
    }
}

static void kp_write(const char *string)
{
    for (const char *ptr = string; *ptr != '\0'; ++ptr)
        kp_put_char(*ptr);
}

static inline void block_forever()
{
    asm("cli");
    asm("hlt");
}

void kp_write_centered(const char *message)
{
    // Find length of messsage
    // Note: We cannot depend on our implementation of strlen here.
    int len;
    const char *ptr = message;
    for (len = 0; *ptr != '\0'; ++ptr, ++len)
        ;
    // Add enough spaces at the start to center message
    const int spacing = (VGA_WIDTH - len) / 2;
    if (spacing > 0)
        for (int i = 0; i < spacing; ++i)
            kp_put_char(' ');
    // Display string anyhow
    kp_write(message);
}

void k_panic(const char *message)
{
    kp_clear();
    kp_write_centered("KERNEL PANIC\n\n");
    kp_write("--------------------------------------------------------------------------------");
    kp_write(message);
    block_forever();
}
