#include <kernel/kbd/comm.h>
#include <kernel/kbd/device.h>
#include <kernel/kbd/kbd_input.h>
#include <kernel/kbd/key_status.h>
#include <kernel/panic.h>
#include <kernel/tty.h>
#include <stdbool.h>
#include <stdint.h>

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