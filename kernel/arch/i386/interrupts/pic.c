#include <kernel/interrupt/pic.h>
#include <kernel/io.h>

/* Port for Master PIC*/
enum PIC_COMMAND_PORT
{
    PIC1_1 = 0x20,
    PIC1_2 = 0xA0
};

enum PIC_DATA_PORT
{
    PIC2_1 = 0x21,
    PIC2_2 = 0xA1
};

/* End Of Interrupt */
enum PIC_COMMAND_CODE
{
    PIC_EOI = 0x20
};

void send_eoi()
{
    outportb(PIC1_1, PIC_EOI);
}

void pic_init()
{
    /*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 PIC2_1	PIC2_2
	*/

    /* ICW1 - begin initialization */
    outportb(PIC1_1, 0x11);
    outportb(PIC1_2, 0x11);

    /* ICW2 - remap offset address of IDT */
    /*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
    outportb(PIC2_1, 0x20);
    outportb(PIC2_2, 0x28);

    /* ICW3 - setup cascading */
    outportb(PIC2_1, 0x00);
    outportb(PIC2_2, 0x00);

    /* ICW4 - environment info */
    outportb(PIC2_1, 0x01);
    outportb(PIC2_2, 0x01);
    /* Initialization finished */

    /* mask interrupts */
    outportb(PIC2_1, 0xff);
    outportb(PIC2_2, 0xff);
}