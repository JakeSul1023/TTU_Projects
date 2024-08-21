%include "io.inc"

section .data
    n1 dd 10
    n2 dd 20
    n3 dd 40
    large dd 0

section .text
global CMAIN

CMAIN:
    mov ebp, esp; for correct debugging
    mov eax, [n1] 
    mov ebx, [n2]
    mov ecx, [n3]

    mov edx, eax    ;edx to hold n1

    ;Compares n1 with n2 and n3
    cmp eax, ebx
    jl not_n1   ;if n1 < n2
    cmp eax, ecx
    jl not_n1   ;if n1 < n3
    jmp set_large   ;n1 > n2 and n3

not_n1:
    cmp ebx, ecx
    jl not_n2   ;if n2 < n3
    mov edx, ebx
    jmp set_large   ;if n2 > n1 and n3

not_n2:
    mov edx, ecx

set_large:    
    mov [large], edx

exit:
    mov eax, 1
    xor ebx, ebx
    int 0x80

