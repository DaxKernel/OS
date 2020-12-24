#include <kernel/kbd.h>
#include <kernel/io.h>
#include <kernel/tty.h>
#include <stdio.h>

// The following type definition does not belong here
// Can be eliminated by implementing stddef.h
typedef unsigned char uint8_t;

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

bool CAPS_LOCK_STATUS = false;

// Returns ASCII string of integer num
void itos(int num, char *buf)
{
    char str[10];
    int i = 0;
    while (num > 0)
    {
        int digit = num % 10;
        str[i] = itoc(digit);
        i += 1;
        num /= 10;
    }
    int j = 0;
    buf[i] = '\0';
    for (i = i - 1; i >= 0; --i)
    {
        buf[j] = str[i];
        j += 1;
    }
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

void kbd_handle(uint8_t code)
{
    switch (code)
    {
    case 0x0:
        printf("Internal buffer overrun\n");
        break;
    case 0xAA:
        printf("Returned during Basic Assurance Test (BAT) after reset. \nAlso L. shift key make code\n");
        break;
    case 0xEE:
        printf("Returned from the ECHO command\n");
        break;
    case 0xF0:
        printf("Prefix of certain make codes (Does not apply to PS/2)\n");
        break;
    case 0xFA:
        printf("Keyboard acknowledge to keyboard command\n");
        break;
    case 0xFC:
        printf("Basic Assurance Test (BAT) failed (PS/2 only)\n");
        break;
    case 0xFD:
        printf("Diagonstic failure (Except PS/2)\n");
        break;
    case 0xFE:
        printf("Keyboard requests for system to resend last command\n");
        break;
    case 0xFF:
        printf("Key error (PS/2 only)\n");
        break;
    }
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
        itos(data, buf);
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

void print_kbd_char(char c)
{
    if (CAPS_LOCK_STATUS)
        c -= 32;
    char str[2];
    str[0] = c;
    str[1] = '\0';
    printf(str);
}

void draw_character(uint8_t data)
{
    switch (data)
    {
    case 0x1E:
        print_kbd_char('a');
        break;
    case 0x30:
        print_kbd_char('b');
        break;
    case 0x2e:
        print_kbd_char('c');
        break;
    case 0x20:
        print_kbd_char('d');
        break;
    case 0x12:
        print_kbd_char('e');
        break;
    case 0x21:
        print_kbd_char('f');
        break;
    case 0x22:
        print_kbd_char('g');
        break;
    case 0x23:
        print_kbd_char('h');
        break;
    case 0x17:
        print_kbd_char('i');
        break;
    case 0x24:
        print_kbd_char('j');
        break;
    case 0x25:
        print_kbd_char('k');
        break;
    case 0x26:
        print_kbd_char('l');
        break;
    case 0x32:
        print_kbd_char('m');
        break;
    case 0x31:
        print_kbd_char('n');
        break;
    case 0x18:
        print_kbd_char('o');
        break;
    case 0x19:
        print_kbd_char('p');
        break;
    case 0x10:
        print_kbd_char('q');
        break;
    case 0x13:
        print_kbd_char('r');
        break;
    case 0x1f:
        print_kbd_char('s');
        break;
    case 0x14:
        print_kbd_char('t');
        break;
    case 0x16:
        print_kbd_char('u');
        break;
    case 0x2f:
        print_kbd_char('v');
        break;
    case 0x11:
        print_kbd_char('w');
        break;
    case 0x2D:
        print_kbd_char('x');
        break;
    case 0x15:
        print_kbd_char('y');
        break;
    case 0x2c:
        print_kbd_char('z');
        break;
    case 0x02:
        printf("1");
        break;
    case 0x03:
        printf("2");
        break;
    case 0x04:
        printf("3");
        break;
    case 0x05:
        printf("4");
        break;
    case 0x06:
        printf("5");
        break;
    case 0x07:
        printf("6");
        break;
    case 0x08:
        printf("7");
        break;
    case 0x09:
        printf("8");
        break;
    case 0x0a:
        printf("9");
        break;
    case 0x0b:
        printf("0");
        break;
    case 0x3a:
        CAPS_LOCK_STATUS = !CAPS_LOCK_STATUS;
        break;
    case 0x39:
        printf(" ");
        break;
    case 0x1c:
        printf("\n");
        break;
    default:
        break;
    }
}

void kbd_draw()
{
    uint8_t status = kbd_read_status();
    if (status & KBD_STATS_MASK_OUT_BUF)
    {
        // printf("time to read\n");
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
        kbd_set_led(false, false, false);
        kbd_enable_interrupts();
    }
    else
    {
        /* TODO: Panic */
    }
}
