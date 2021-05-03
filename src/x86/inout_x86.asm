[bits 32]

VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x02

; dl = character to print
; ecx = index on the screen
print_char_x86:
    pusha
    mov al, dl
    mov ah, WHITE_ON_BLACK
    add ecx, VIDEO_MEMORY
    mov [ecx], ax
    popa
    ret

; ebx = string's address
; set ecx to 0 before calling
print_string_x86:
    pusha

    mov dl, [ebx]
    cmp dl, 0
    je string_x86_eq
    call print_char_x86
    add ebx, 1
    add ecx, 2
    jmp print_string_x86 + 1


string_x86_eq:
    popa
    ret