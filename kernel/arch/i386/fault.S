# Produces CPU Exceptions for testing

.intel_syntax noprefix
.global overflow 

overflow:
    pushfd
    bts dword ptr [esp], 11
    popfd
    into
    ret