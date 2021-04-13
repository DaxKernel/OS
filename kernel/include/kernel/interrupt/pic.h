#ifndef PIC_H
#define PIC_H

#include <kernel/io.h>
/* Port for Master PIC*/
const int PIC1 = 0x20;

/* End Of Interrupt */
const int PIC_EOI = 0x20;

inline void send_eoi(){
    outportb(PIC1, PIC_EOI);
}

#endif