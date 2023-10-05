BITS 32
SYS_EXIT equ 1
SYS_READ equ 3
SYS_WRITE equ 4
STDIN equ 0
STDOUT equ 1

segment .data
    msg1 db "Please enter a single digit: "
    len1 equ $- msg1

    msg2 db "Please enter another single digit: "
    len2 equ $- msg2

    msg3 db "The quotient is: "
    len3 equ $- msg3

    msg4 db "The remainder is: "
    len4 equ $- msg4

    ;Trying a newline segment
    newline db 0xA
    newline_len equ $- newline

segment .bss
    num1 resb 2 ;User value 1
    num2 resb 2 ;User value 2
    res_q resb 2    ;Quotient
    res_r resb 2    ;Remainder

section .text
    GLOBAL _start

_start:
    ;Reads out the first message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg1
    mov edx, len1
    int 0x80

    ;Reads in the first message
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, num1
    mov edx, 2
    int 0x80 

    ;Reads out the second message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg2
    mov edx, len2
    int 0x80
    
    ;Reads in the second number
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, num2
    mov edx, 2
    int 0x80

    ;Stores num# into AX and BX then converts them to int
    ;Tests if BL is 0 if so then EXIT
    ;Divides then converts to string and stores
    movzx ax, BYTE [num1]
    sub al, '0'
    movzx bx, BYTE [num2]
    sub bl, '0'
    test bl, bl
    jz exit
    idiv bl 
    add al, '0'
    mov [res_q], al
    add ah, '0'
    mov [res_r], ah

    ;Reads out the third message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg3
    mov edx, len3 
    int 0x80

    ;Reads out the quotient
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, res_q
    mov edx, 1
    int 0x80

    ; Print newline
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, newline
    mov edx, newline_len
    int 0x80

    ;Reads out the fourth message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg4
    mov edx, len4
    int 0x80

    ;Reads out the remainder
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, res_r
    mov edx, 1
    int 0x80

    ; Print newline
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, newline
    mov edx, newline_len
    int 0x80

exit:
    ; Exit the program
    mov eax, SYS_EXIT
    xor ebx, ebx    ; Return 0 status
    int 0x80