#include <kernel/kbd/comm.h>
#include <kernel/kbd/device.h>
#include <kernel/kbd/kbd.h>
#include <kernel/kbd/kbd_callback.h>
#include <kernel/kbd/kbd_input.h>
#include <kernel/kbd/kbd_table.h>
#include <kernel/kbd/key_status.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool handle_special_key_states(uint8_t scancode)
{

    switch (scancode)
    {
    case 0x3a:
        KEY_STATUS.CAPS_LOCK = !KEY_STATUS.CAPS_LOCK;
        break;
    case 0x45:
        KEY_STATUS.NUM_LOCK = !KEY_STATUS.NUM_LOCK;
        break;
    case 0x46:
        KEY_STATUS.SCROLL_LOCK = !KEY_STATUS.SCROLL_LOCK;
        break;
    default:
        return false;
    }
    return true;
}

void handle_character(uint8_t scancode)
{
    if (!handle_special_key_states(scancode))
    {
        char key = kbd_scan_tbl[scancode];
        if (key == '\0')
            return;
        // Capitalize key if CAPS_LOCK
        key = KEY_STATUS.CAPS_LOCK && key != '\b' ? key - 32 : key;

        // Optional callback
#ifdef ENABLE_KBD_CALLBACK
        callback(key);
#endif

        kbd_insert((unsigned char)key);
    }
}

void kbd_handle()
{
    uint8_t status = kbd_read_status();
    if (status & KBD_STATS_MASK_OUT_BUF)
    {
        uint8_t data;
        data = kbd_read_data();
        handle_character(data);
    }
}
