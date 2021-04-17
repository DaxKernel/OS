#ifndef KBD_COMM_H
#define KBD_COMM_H

#include <stdint.h>

uint8_t kbd_read_status();
uint8_t kbd_read_data();
void kbd_data_send_cmd(uint8_t cmd);
void kbd_ack();
void kbd_ctrl_send_cmd(uint8_t cmd);
void kbd_enable_interrupts();

#endif