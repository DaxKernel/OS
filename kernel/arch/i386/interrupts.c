#include <kernel/io.h>

#define IDT_SIZE 256

struct IDT_entry
{
    unsigned short int offset_lowerbits;
    unsigned short int selector;
    unsigned char zero;
    unsigned char type_attr;
    unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

void idt_init(void)
{
    unsigned long keyboard_address;
    unsigned long idt_address;
    unsigned long idt_ptr[2];

    extern int keyboard_handler();
    extern int load_idt();

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
    idt_address = (unsigned long)IDT;
    idt_ptr[0] = (sizeof(struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
    idt_ptr[1] = idt_address >> 16;

    load_idt(idt_ptr);
}