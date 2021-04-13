#include <kernel/interrupt/pic.h>
#include <kernel/kbd.h>
#include <kernel/interrupt/irq_handlers.h>
#include <kernel/panic.h>

IRQ_HANDLER keyboard_handler(IRQ_ARG)
{
    /* write EOI */
    send_eoi();
    kbd_draw();
}

IRQ_HANDLER divide_by_zero(IRQ_ARG)
{
    send_eoi();
    k_panic("Divide by zero!");
}