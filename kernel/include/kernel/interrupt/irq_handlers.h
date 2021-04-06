#ifndef IRQ_HANDLERS_H
#define IRQ_HANDLERS_H

#include <stdint.h>
struct interrupt_frame
{
    uint32_t error_code;
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
};

#define IRQ_HANDLER __attribute__((interrupt)) void
#define IRQ_ARG __attribute__((unused)) struct interrupt_frame *frame

void keyboard_handler();
#endif