/**
WARNING : This is an auto-generated file.
Any modifications must be made in the script configs.
Find them @ <project_root>/misc/exception
**/

#include <kernel/interrupt/exceptions.h>
#include <kernel/panic.h>
#include <kernel/interrupt/pic.h>

IRQ_HANDLER cpu_exception_0(IRQ_ARG)
{
	send_eoi();
	k_panic("Divide By Zero");
}

IRQ_HANDLER cpu_exception_1(IRQ_ARG)
{
	send_eoi();
	k_panic("Debug");
}

IRQ_HANDLER cpu_exception_2(IRQ_ARG)
{
	send_eoi();
	k_panic("Non-Maskable Interrupts");
}

IRQ_HANDLER cpu_exception_3(IRQ_ARG)
{
	send_eoi();
	k_panic("Breakpoint");
}

IRQ_HANDLER cpu_exception_4(IRQ_ARG)
{
	send_eoi();
	k_panic("Overflow");
}

IRQ_HANDLER cpu_exception_5(IRQ_ARG)
{
	send_eoi();
	k_panic("Bound Range Exceeded");
}

IRQ_HANDLER cpu_exception_6(IRQ_ARG)
{
	send_eoi();
	k_panic("Invalid Opcode");
}

IRQ_HANDLER cpu_exception_7(IRQ_ARG)
{
	send_eoi();
	k_panic("Device Not Available");
}

