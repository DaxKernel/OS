#include <kernel/io.h>
#include <stdint.h>
#include <kernel/interrupts/irq_handlers.h>

enum
{
    IDT_SIZE = 256
};

struct IDT_entry
{
    uint16_t offset_lowerbits;
    uint16_t selector;
    unsigned char zero;
    unsigned char type_attr;
    uint16_t offset_higherbits;
} IDT[IDT_SIZE];

static inline void load_idt()
{
    typedef struct __attribute__((packed))
    {
        uint16_t limit;
        uintptr_t start;
    } IDT_REPRESENTATION;

    const IDT_REPRESENTATION rep = {(sizeof(struct IDT_entry) * (IDT_SIZE - 1)), (uintptr_t)IDT};

    asm("lidt %0; sti;"
        : /*No Output*/
        : "m"(*(IDT_REPRESENTATION *)&rep));
}

void idt_init(void)
{
    /* populate IDT entry of keyboard's interrupt */
    IDT[0x21].offset_lowerbits = (uint32_t)keyboard_handler_main & 0xffff;
    IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[0x21].offset_higherbits = (uint32_t)keyboard_handler_main >> 16;

    /*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

    /* ICW1 - begin initialization */
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);

    /* ICW3 - setup cascading */
    outportb(0x21, 0x00);
    outportb(0xA1, 0x00);

    /* ICW4 - environment info */
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    outportb(0x21, 0xff);
    outportb(0xA1, 0xff);

    /* fill the IDT descriptor */
    load_idt();
}