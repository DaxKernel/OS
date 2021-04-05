.global keyboard_handler

keyboard_handler:
    pusha                 
	call keyboard_handler_main
    popa
	iret