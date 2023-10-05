BITS 32

SYS_EXIT equ 1
SYS_READ equ 3
SYS_WRITE equ 4
STDIN equ 0
STDOUT equ 1

segment .data
    msg1 db "The Swapping Program", 0xA
    len1 equ $- msg1

    msg2 db "Please enter two character strings: "
    len2 equ $- msg2

    msg3 db "The answer is: "
    len3 equ $- msg3

segment .bss
    var resb 3  ;variable for user input

section .text
    GLOBAL _start

_start:
    ;Reads out the first message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg1
    mov edx, len1
    int 0x80

    ;Reads out the second message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg2
    mov edx, len2
    int 0x80

    ;Reads in the user input
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, var
    mov edx, 2
    int 0x80

    ;Stores first byte of var in AL 
    ;Stores second byte into BL
    ;Swaps them then stores into var
    mov al, [var]
    mov bl, [var+1]
    mov [var], bl
    mov [var+1], al

    mov BYTE [var+2], 0xA

    ;Reads out the third message 
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg3
    mov edx, len3
    int 0x80

    ;Reads out the answer
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, var
    mov edx, 3
    int 0x80

exit:
    ;Exits the program
    mov eax, SYS_EXIT
    xor ebx, ebx
    int 0x80