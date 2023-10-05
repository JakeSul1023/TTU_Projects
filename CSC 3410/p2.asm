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

    msg3 db "The product is: "
    len3 equ $- msg3

segment .bss 
    num1 resb 2 ;first variable 
    num2 resb 2 ;second variable
    res resb 2  ;Results

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

    ;Reads out the third message
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, msg3
    mov edx, len3 
    int 0x80

    ;Loads the num# into EAX and EBX extended
    ;Converts them from ASCII to int
    ;Multiplys then converts back to string
    movzx eax, byte [num1]
    sub eax, '0'
    movzx ebx, byte [num2]
    sub ebx, '0'
    imul ebx
    add eax, '0'
    mov [res], al
    mov BYTE [res+1], 10
    
    ;Reads out the results to the screen 
    mov eax, SYS_WRITE
    mov ebx, STDOUT
    mov ecx, res
    mov edx, 2
    int 0x80

exit:
    ;Exits the program
    mov eax, SYS_EXIT
    xor ebx, ebx
    int 0x80