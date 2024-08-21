%include "io.inc"

section .text
    global CMAIN

CMAIN:
    mov ebp, esp; for correct debugging
    mov ecx, 1  ;iterator
    xor eax, eax;running total

while_loop:
    cmp ecx, 10 ;compare iterator
    jg end_while;iteraor > 10 break
    add eax, ecx
    inc ecx     ;increment iterator
    jmp while_loop  

end_while:      ;exit
    mov eax, 1
    xor ebx, ebx
    int 0x80