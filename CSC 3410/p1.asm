BITS 32
SYS_EXIT equ 1
SYS_READ equ 3
SYS_WRITE equ 4
STDIN equ 0
STDOUT equ 1

segment .data

    msg1 db "Please enter a single digit number: "
    len1 equ $- msg1

    msg2 db "Please enter a single digit number: "
    len2 equ $- msg2

    msg3 db "The sum is: "
    len3 equ $- msg3

segment .bss
    ;Variables
    num1 resb 2
    num2 resb 2
    res resb 1
    nl resb 1

section .text
    global _start

_start:
    ;Reads out the first message
    mov eax, SYS_WRITE
    mov ebx, STDOUT 
    mov ecx, msg1
    mov edx, len1
    int 0x80
    ;Reads in the user input
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
    ;Reads in the second input
    mov eax, SYS_READ
    mov ebx, STDIN
    mov ecx, num2
    mov edx, 2
    int 0x80
    ;Reads out the third message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg3
    mov edx, len3 
    int 0x80

    ;Converts num1 to a number
    mov eax, [num1]
    sub eax, '0'

    ;Converts num2 to a number
    mov ebx, [num2]
    sub ebx, '0'

    ;Adds the numbers and converts it back to string
    add eax, ebx
    add eax, '0'
    mov [res], eax
    mov BYTE [nl], 10

    ;Prints te results to the screen
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, res
    mov edx, 2
    int 0x80

exit:
    ;Exits the program
    mov eax, SYS_EXIT
    xor ebx, eax
    int 0x80
