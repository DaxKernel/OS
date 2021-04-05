.global keyboard_handler
.global load_idt

keyboard_handler:
    pusha                 
	call keyboard_handler_main
    popa
	iret
