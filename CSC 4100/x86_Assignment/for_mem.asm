%include "io.inc"

section .data
    sum dd 0
    count dd 10
    const dd 1

section .text
    global CMAIN

CMAIN:
    mov ebp, esp; for correct debugging
    xor eax, eax
    inc eax

loop: 
    ;dec counter
    mov ebx, [count]
    dec ebx
    mov [count], ebx

    cmp ebx, 0  ;if 0 break
    je exit
    
    ;Add current num to sum
    add [sum], eax
    inc eax
    jmp loop    ;repeat

exit:
    ;exit program   
    mov eax, 1
    xor ebx, ebx
    int 0x80