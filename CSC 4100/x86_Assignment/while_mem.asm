%include "io.inc"

section .data
    sum dd 0
    count dd 1
    max dd 10

section .text
    global CMAIN

CMAIN:
    mov ebp, esp; for correct debugging

while_loop:
    mov eax, [count]
    cmp eax, [max]
    jg exit

    mov ebx, [sum]
    add ebx, eax
    mov [sum], ebx

    inc dword [count]

    jmp while_loop

exit:
    mov eax, 1
    xor ebx, ebx
    int 0x80