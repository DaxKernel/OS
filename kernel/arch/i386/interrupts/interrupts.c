#include <kernel/io.h>
#include <stdint.h>
#include <kernel/interrupt/irq_handlers.h>
#include <kernel/interrupt/pic.h>
#include <kernel/interrupt/exceptions.h>

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

void add_interrupt(const int intr, uint32_t handler)
{
    /* populate IDT entry of keyboard's interrupt */
    IDT[intr].offset_lowerbits = (uint32_t)handler & 0xffff;
    IDT[intr].selector = 0x08; /* KERNEL_CODE_SEGMENT_OFFSET */
    IDT[intr].zero = 0;
    IDT[intr].type_attr = 0x8e; /* INTERRUPT_GATE */
    IDT[intr].offset_higherbits = ((uint32_t)handler & 0xffff0000) >> 16;
}

void idt_init(void)
{
    pic_init();

    /* IRQ1 for PS/2 Keyboard */
    add_interrupt(0x21, (uint32_t)keyboard_handler);

    /* Divide by zero - CPU Exception */
    // add_interrupt(0x0, (uint32_t)divide_by_zero);
    add_interrupt(0, (uint32_t)cpu_exception_0);
    add_interrupt(1, (uint32_t)cpu_exception_1);
    add_interrupt(2, (uint32_t)cpu_exception_2);
    add_interrupt(3, (uint32_t)cpu_exception_3);
    add_interrupt(4, (uint32_t)cpu_exception_4);
    add_interrupt(5, (uint32_t)cpu_exception_5);
    add_interrupt(6, (uint32_t)cpu_exception_6);
    add_interrupt(7, (uint32_t)cpu_exception_7);

    load_idt();
}