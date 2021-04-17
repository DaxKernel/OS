#include <kernel/io.h>
#include <kernel/kbd/device.h>
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

void kbd_ack()
{
    while (!(inportb(0x60) == 0xfa))
        ;
}

void kbd_ctrl_send_cmd(uint8_t cmd)
{

    //! wait for kkybrd controller input buffer to be clear
    while (1)
        if ((kbd_read_status() & KBD_STATS_MASK_IN_BUF) == 0)
            break;

    outportb(KBD_CMD_REG, cmd);
}
void kbd_enable_interrupts()
{
    /* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
    outportb(0x21, 0xFD);
}