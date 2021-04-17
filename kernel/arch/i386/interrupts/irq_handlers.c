#include <kernel/interrupt/pic.h>
#include <kernel/kbd/kbd.h>
#include <kernel/interrupt/irq_handlers.h>
#include <kernel/panic.h>

IRQ_HANDLER keyboard_handler(IRQ_ARG)
{
    /* write EOI */
    send_eoi();
    kbd_handle();
}

IRQ_HANDLER divide_by_zero(IRQ_ARG)
{
    send_eoi();
    k_panic("Divide by zero!");
}