# Produces CPU Exceptions for testing

.intel_syntax noprefix
.global overflow 
.global div_by_zero 

overflow:
    pushfd
    bts dword ptr [esp], 11
    popfd
    into
    ret

div_by_zero:
    mov edx, 0
    mov eax, 0
    mov ecx, 0
    div ecx
    ret