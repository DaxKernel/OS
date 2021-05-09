# Declare constants for the multiboot header.
.set ALIGN,    1<<0             # align loaded modules on page boundaries
.set MEMINFO,  1<<1             # provide memory map
.set VBEINFO,  1<<2				# provide vbe info
.set FLAGS,    ALIGN | MEMINFO | VBEINFO  # this is the Multiboot 'flag' field
.set MAGIC,    0x1BADB002       # 'magic number' lets bootloader find the header
.set CHECKSUM, -(MAGIC + FLAGS) # checksum of above, to prove we are multiboot
.set MODE_TYPE, 0
.set WIDTH, 1024 
.set HEIGHT, 768
.set DEPTH, 32

# Declare a header as in the Multiboot Standard.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.skip 24
.long MODE_TYPE
.long WIDTH
.long HEIGHT
.long DEPTH

# Reserve a stack for the initial thread.
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

# The kernel entry point.
.section .text
.global _start
.type _start, @function
_start:
	movl $stack_top, %esp

	# Push EAX and EBX to get memory map from GRUB/Multiboot
	pushl %eax
	pushl %ebx

	# Call the global constructors.
	call _init

	# Transfer control to the main kernel.
	call kernel_main

	# Hang if kernel_main unexpectedly returns.
	# cli
1:	hlt
	jmp 1b
.size _start, . - _start
