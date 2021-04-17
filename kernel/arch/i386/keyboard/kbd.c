#include <kernel/kbd.h>
#include <kernel/io.h>
#include <kernel/tty.h>
#include <kernel/kbd_input.h>
#include <kernel/kbd_table.h>
#include <kernel/kbd_callback.h>
#include <kernel/panic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// Data I/O must accessed through port 0x60
enum KBD_DATA_IO
{
    KBD_DATA_INPUT_BUF = 0x60,
    KBD_DATA_CMD_REG = 0x60
};

// Communication to the PS/2 controller should happen through port 0x64
enum KBD_CMD_IO
{
    KBD_STATS_REG = 0x64,
    KBD_CMD_REG = 0x64
};

// Use these bitmasks to find out keyboard status
enum KBD_STATS_MASK
{
    KBD_STATS_MASK_OUT_BUF = 1,    //00000001
    KBD_STATS_MASK_IN_BUF = 2,     //00000010
    KBD_STATS_MASK_SYSTEM = 4,     //00000100
    KBD_STATS_MASK_CMD_DATA = 8,   //00001000
    KBD_STATS_MASK_LOCKED = 0x10,  //00010000
    KBD_STATS_MASK_AUX_BUF = 0x20, //00100000
    KBD_STATS_MASK_TIMEOUT = 0x40, //01000000
    KBD_STATS_MASK_PARITY = 0x80   //10000000
};

// Returns ASCII code for digit i
char itoc(int i)
{
    return i + '0';
}

struct
{
    bool CAPS_LOCK;
    bool NUM_LOCK;
    bool SCROLL_LOCK;
} KEY_STATUS;

void set_key_status(bool CAPS_LOCK, bool NUM_LOCK, bool SCROLL_LOCK)
{
    KEY_STATUS.CAPS_LOCK = CAPS_LOCK;
    KEY_STATUS.NUM_LOCK = NUM_LOCK;
    KEY_STATUS.SCROLL_LOCK = SCROLL_LOCK;
}

uint8_t kbd_read_status()
{
    return inportb(KBD_STATS_REG);
}

uint8_t kbd_read_data()
{

    return inportb(KBD_DATA_INPUT_BUF);
}

void kbd_data_send_cmd(uint8_t cmd)
{

    //! wait for kkybrd controller input buffer to be clear
    while (1)
        if ((kbd_read_status() & KBD_STATS_MASK_IN_BUF) == 0)
            break;

    //! send command byte to kybrd encoder
    outportb(KBD_DATA_CMD_REG, cmd);
}

void kbd_ack(void)
{
    while (!(inportb(0x60) == 0xfa))
        ;
}

void kbd_set_led(bool num, bool caps, bool scroll)
{
    const int KYBRD_ENC_CMD_SET_LED = 0xED;
    uint8_t data = 0;

    //! set or clear the bit
    data = (scroll) ? (data | 1) : (data & 1);
    data = (num) ? (num | 2) : (num & 2);
    data = (caps) ? (num | 4) : (num & 4);

    //! send the command -- update keyboard Light Emetting Diods (LEDs)
    kbd_data_send_cmd(KYBRD_ENC_CMD_SET_LED);
    kbd_ack();
    kbd_data_send_cmd(data);
    kbd_ack();
}

void kbd_ctrl_send_cmd(uint8_t cmd)
{

    //! wait for kkybrd controller input buffer to be clear
    while (1)
        if ((kbd_read_status() & KBD_STATS_MASK_IN_BUF) == 0)
            break;

    outportb(KBD_CMD_REG, cmd);
}

bool kbd_self_test()
{
    const int KYBRD_CTRL_CMD_SELF_TEST = 0xAA;

    //! send command
    kbd_ctrl_send_cmd(KYBRD_CTRL_CMD_SELF_TEST);

    //! wait for output buffer to be full
    while (1)
        if (kbd_read_status() & KBD_STATS_MASK_OUT_BUF)
            break;

    //! if output buffer == 0x55, test passed
    return (kbd_read_data() == 0x55) ? true : false;
}

void kbd_set_scancode()
{

    printf("Trying to set scancode = 1\n");
    // Set scancode set
    kbd_data_send_cmd(0xF0);
    kbd_ack();
    kbd_data_send_cmd(0x01);
    kbd_ack();
    printf("Apparently the keyboard did accept our scan-code.\n");

    // Confirm scan code set
    kbd_data_send_cmd(0xF0);
    kbd_data_send_cmd(0);
    kbd_ack();
    kbd_ack();
    uint8_t data = kbd_read_data();
    char buf[10];
    switch (data)
    {
    case 1:
        printf("Scan code 1 is in use\n");
        break;
    case 2:
        printf("Scan code 2 is in use\n");
        break;
    case 3:
        printf("Scan code 3 is in use\n");
        break;
    default:
        itoa(data, buf);
        printf("Response is %s\n", buf);
        break;
    }
}

void kbd_flush_buffer()
{
    while (kbd_read_status() & KBD_STATS_MASK_OUT_BUF)
        kbd_read_data();
}

bool kbd_initialize()
{
    // Initialize the PS/2 controller

    // Disable device
    kbd_ctrl_send_cmd(0xAD);
    kbd_ack();
    kbd_ctrl_send_cmd(0xA7);

    // Flush output buffer
    kbd_flush_buffer();

    // Perform controller self test
    if (!kbd_self_test())
        return false;

    // Enable device
    kbd_ctrl_send_cmd(0xAE);
    kbd_ctrl_send_cmd(0xA8);

    // Reset keyboard device
    // Usually 0xFF gives 0xFA on pass but sometimes you need to,
    // Send 0xFF , wait for ACK and then check output of auto initiated self-test
    kbd_data_send_cmd(0xFF);
    uint8_t resp = kbd_read_data();
    switch (resp)
    {
    case 0xFA:
        // printf("Keyboard device reset successfully. \n");
        break;
    case 0xFC:
        // printf("Keyboard device reset failed :/ .\n");
        return false;
        break;
    default:
        // printf("No PS/2 Device ??? \n");
        return false;
        break;
    }

    return true;
}

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

void draw_character(uint8_t scancode)
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

void kbd_draw()
{
    uint8_t status = kbd_read_status();
    if (status & KBD_STATS_MASK_OUT_BUF)
    {
        uint8_t data;
        data = kbd_read_data();
        draw_character(data);
    }
}

void kbd_enable_interrupts()
{
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    outportb(0x21, 0xFD);
}

void kbd_init()
{
    if (kbd_initialize())
    {
        tty_print_success("Keyboard Status", "OK");
        kbd_buffer_init();
        kbd_set_led(false, false, false);
        set_key_status(false, false, false);
        kbd_enable_interrupts();
    }
    else
    {
        k_panic("Keyboard init failed!!");
    }
}
