BITS 32

segment .data

    msg0 db "Please enter a string:", 0xA
    len0 equ $- msg0

    msg1 db "It is a palindrome", 0xA
    len1 equ $- msg1

    msg2 db "It is NOT a palindrome", 0xA
    len2 equ $- msg2

segment .bss

    userStr resb 1024    ; Reserve space for userStr

segment .text

    GLOBAL _start

_start:
    ; Print msg0
    mov eax, 4              
    mov ebx, 1             
    mov ecx, msg0           
    mov edx, len0           
    int 0x80                

    ; Read user input
    mov eax, 3              
    mov ebx, 0              
    mov ecx, userStr
    mov edx, 1024
    int 0x80

    ; Checks if user input is 0
    cmp BYTE[userStr], 0    
    je exit                 

    ; Adjust input length for character exclusion
    sub eax, 2

    ; Push eax and the userStr onto stack
    push eax
    push userStr

    ; Call is_palin function
    call palin_load
    add esp, 8          ; Restore the stack
    cmp eax, 1          ; Checking if its palindrome
    jne not_palin  

    ; Print msg1
    mov eax, 4
    mov ebx, 1
    mov ecx, msg1
    mov edx, len1
    int 0x80
    
    ; Loop, restart program
    jmp _start

not_palin:

    ; Print msg2
    mov eax, 4
    mov ebx, 1
    mov ecx, msg2
    mov edx, len2
    int 0x80

    jmp _start

exit:

    mov eax, 1
    mov ebx, 0
    int 0x80

palin_load:

    push ebp                ; Push base pointer
    mov ebp, esp            ; Establish new base pointer
    pusha                   ; Save register values

    mov eax, [ebp + 8]      ; Load address of userStr from stack
    mov ebx, [ebp + 12]     ; Load length of string from stack

    mov ecx, eax            ; Copy address of buffer
    add ecx, ebx            ; Calculate end address of string

palin_loop:

    cmp eax, ecx            ; Compare current & end addresses
    jge end_palin   

    mov dl, [eax]           ; Load character from front of string
    mov dh, [ecx]           ; Load character from end of string

    cmp dl, dh                  
    jne end_not_palin     

    inc eax                 ; Increment to next character from start
    dec ecx                 ; Decrement to previous character from end
    jmp palin_loop          ; Jump to repeat loop

end_palin:

    popa                    ; Restore register
    pop ebp                 ; Restore base pointer
    mov eax, 1              ; Return 1 to indicate a palindrome
    ret                     

end_not_palin:

    popa
    pop ebp
    mov eax, 0
    ret