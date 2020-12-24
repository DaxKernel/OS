.global keyboard_handler
.global load_idt

keyboard_handler:
    pusha                 
	call keyboard_handler_main
    popa
	iret

load_idt:
	movl 4(%esp),%eax
	lidt (%eax)
	sti
	ret

