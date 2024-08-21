%include "io.inc"

segment .data
    n1 dd 10
    n2 dd 20
    n3 dd 40
    large dd 0

segment .text
    global CMAIN

CMAIN:
    mov ebp, esp; for correct debugging
    ;Assume n1 is largest
    mov eax, [n1]
    mov [large], eax

    ;Compare n2 and n1
    mov ebx, [n2]
    cmp ebx, eax
    jle n3_check    ;if n2 > n1 check n3
    mov [large], ebx;update largest value

n3_check:
    ;Compare n3 with n2 or n1
    mov ecx, [n3]
    cmp ecx, [large]
    jle exit
    mov [large], ecx;Update largest val

exit:
    ;Exit
    mov eax, 1
    xor ebx, ebx
    int 0x80