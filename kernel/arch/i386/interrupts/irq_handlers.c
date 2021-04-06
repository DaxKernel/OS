#include <kernel/io.h>
#include <kernel/kbd.h>
#include <kernel/interrupt/irq_handlers.h>

IRQ_HANDLER keyboard_handler_main(IRQ_ARG)
{
    /* write EOI */
    outportb(0x20, 0x20);
    kbd_draw();
}