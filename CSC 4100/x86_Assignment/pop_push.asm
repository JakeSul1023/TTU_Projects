%include "io.inc"

section .data
    val1 dd 123
    val2 dd 456

section .text
    global CMAIN

CMAIN:
    mov ebp, esp; for correct debugging         ;swtiches values in variables
    mov eax, [val1]
    mov ebx, [val2]

    push ebx    ;push 456
    push eax    ;push 123

    pop ebx     ;store 123 in ebx
    pop eax     ;store 456 in eax

    ;exit
    mov eax, 1
    xor ebx, ebx
    int 0x80