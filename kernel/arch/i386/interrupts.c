#include <kernel/io.h>
#include <stdint.h>

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

struct IDT_REPRESENTATION
{
    uint16_t limit;
    uintptr_t start;
} __attribute__((packed));

struct IDT_REPRESENTATION LIDT_IDT_REP;

void load_idt(struct IDT_REPRESENTATION *rep)
{
    asm("lidt %0; sti;"
        : /*No Output*/
        : "m"(*rep));
}

void idt_init(void)
{
    unsigned long keyboard_address;

    extern int keyboard_handler();

    /* populate IDT entry of keyboard's interrupt */
    keyboard_address = (unsigned long)keyboard_handler;
    IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
    IDT[0x21].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[0x21].zero = 0;
    IDT[0x21].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[0x21].offset_higherbits = (keyboard_address & 0xffff0000) >> 16;

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
    LIDT_IDT_REP.limit = (sizeof(struct IDT_entry) * (IDT_SIZE - 1));
    LIDT_IDT_REP.start = (uintptr_t)IDT;
    load_idt(&LIDT_IDT_REP);
}