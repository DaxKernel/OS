#ifndef KBD_DEVICE_H
#define KBD_DEVICE_H

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

void kbd_init();

#endif