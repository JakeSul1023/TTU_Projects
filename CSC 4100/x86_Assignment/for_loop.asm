%include "io.inc"

segment .text
GLOBAL CMAIN

CMAIN:
    mov ebp, esp; for correct debugging
    mov ecx, 10     ;Loop iterator
    xor eax, eax    ;Clear for sum total
    xor ebx, ebx    ;Clear for current num

loop:
    inc ebx         ;Inc by 1 for next num
    add eax, ebx    ;Add current num to eax
    dec ecx         ;Dec iterator
    jnz loop        ;If != 0 repeat

    ;Exit
    mov eax, 1  
    xor ebx, ebx
    int 0x80