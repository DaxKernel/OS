/*
Global Descriptor Table
Our code segment and data segment overlap completely.
This will give us a nice flat memory model to work with.
<rmidhunsuresh@gmail.com>

Thanks to Brandon Friesen for his guide on GDT programming which can be found at
http://www.osdever.net/bkerndev/Docs/gettingstarted.htm
*/

struct gdt_entry
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed));

struct gdt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

/* Global Descriptor Table, with 3 entries  */
struct gdt_entry gdt[3];

/* GDT pointer */
struct gdt_ptr gp;

/* This function is defined externally in l_gdt.S assembly file */
extern void load_gdt(struct gdt_ptr *);

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

/* Should be called from  kernel_main */
void gdt_init()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 3) - 1;
    gp.base = (unsigned int)&gdt;

    /* Our NULL descriptor
       This descriptor is necessary as per standard.
     */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* Code Segment */
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    /* Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    /* Load the GDT*/
    load_gdt(&gp);
}
