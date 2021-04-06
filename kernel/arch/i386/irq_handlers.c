#include <kernel/io.h>
#include <kernel/kbd.h>
#include <stdio.h>
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

IRQ_HANDLER keyboard_handler_main(IRQ_ARG)
{
    /* write EOI */
    outportb(0x20, 0x20);
    kbd_draw();
}