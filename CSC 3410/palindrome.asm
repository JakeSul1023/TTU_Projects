BITS 32
SYS_EXIT equ 1
SYS_READ equ 3
SYS_WRITE equ 4
STDIN equ 0
STDOUT equ 1

segment .data
    msg1 db "Please enter a string: "
    len1 equ $- msg1

    msg2 db "It is a palindrome"
    len2 equ $- msg2

    msg3 db "It is NOT a palindrome"
    len3 equ $- msg3

    msg4 db 0xa ;New line
    len4 equ $- msg4

segment .bss
    userStr resb 1024   ;User string input

section .text 
    GLOBAL _start

_start:
    ;Input string message
    mov eax, 4
    mov ebx, 1
    mov ecx, msg1
    mov edx, len1
    int 0x80

    ;New line
    mov eax, 4
    mov ebx, 1
    mov ecx, msg4
    mov edx, len4
    int 0x80

    ;User input 
    mov eax, 3
    mov ebx, 0
    mov ecx, userStr
    mov edx, 1024
    int 0x80

    ;Checks for no data if yes end
    cmp BYTE[userStr], 0xa
    je end

    dec eax ;Adjust for the newline
    lea edx, [userStr + eax -1] ;Address for the last character
    lea ecx, [userStr]          ;Base address
    push edx
    push ecx
    call palinSetup   ;Calls the palindrome checker

    ;New line
    mov eax, 4
    mov ebx, 1
    mov ecx, msg4
    mov edx, len4
    int 0x80

    jmp _start  ;Restarts loop

palinSetup:
    mov ebp, esp       ;Set up stack
    mov ecx, [ebp + 4] ;Base address of the string
    mov edx, [ebp + 8] ;Address of the last character

compLoop:
    mov al, [ecx]   ;Base address character into al
    mov bl, [edx]   ;Last character address into bl
    cmp al, bl      ;Comparing characters
    jne notPalin    ;Ends if not equal
    inc ecx         ;Moves pointer
    dec edx         ;Moves pointer
    cmp ecx, edx    ;Compares pointer
    jle compLoop    ;Continues loop till pointers cross

yesPalin:
    ;Displays if it is a palindrome      
    mov eax, 4
    mov ebx, 1
    mov ecx, msg2
    mov edx, len2
    int 0x80
    ret

notPalin:
    ;Displays if it's not a palindrome
    mov eax, 4
    mov ebx, 1
    mov ecx, msg3
    mov edx, len3
    int 0x80
    ret ;jmp _start

end:
    ;Ends the program
    mov eax, 1
    xor ebx, ebx
    int 0x80    