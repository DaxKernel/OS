#include <kernel/io.h>
#include <kernel/kbd.h>
#include <kernel/interrupt/irq_handlers.h>
#include <kernel/panic.h>

IRQ_HANDLER keyboard_handler(IRQ_ARG)
{
    /* write EOI */
    outportb(0x20, 0x20);
    kbd_draw();
}

IRQ_HANDLER divide_by_zero(IRQ_ARG)
{
    outportb(0x20, 0x20);
    k_panic("Divide by zero!");
}