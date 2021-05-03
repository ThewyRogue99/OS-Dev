[bits 16]
; Move the character to al register to print
print_char_16_bit:
    pusha
    mov ah, 0x0e
    int 0x10
    popa
    ret

; Move the string's addres to bx to print
print_string_16_bit:
    pusha
    mov al, [bx]
    cmp al, 0
    je string_16bit_eq
    call print_char_16_bit
    add bx, 1
    jmp print_string_16_bit + 1

string_16bit_eq:
    popa
    ret

; Move hex value's address to bx to print
; Move the byte length of hex value to dx
print_hex_16bit:
    pusha

    mov cx, 1
loop:
    call print_hex_byte_16bit
    cmp cx, dx
    jne count_not_eq

    popa
    ret

count_not_eq:
    add bx, 1
    add cx, 1
    mov al, ' '
    call print_char_16_bit
    jmp loop

; Move hex value's address to bx to print
print_hex_byte_16bit:
    pusha

    mov ah, [bx] ; Set registers
    mov dl, ah
    and dl, 0x0F
    mov dh, ah
    shr dh, 4

    cmp dh, 9 ; Compare registers
    jg if_dh_16_bigger
    add dh, 48
dh_check_end:
    cmp dl, 9
    jg if_dl_16_bigger
    add dl, 48
dl_check_end:
    mov al, dh
    call print_char_16_bit
    mov al, dl
    call print_char_16_bit

    popa
    ret

if_dh_16_bigger:
    add dh, 55
    jmp dh_check_end

if_dl_16_bigger:
    add dl, 55
    jmp dl_check_end


; dl = selected drive (0 based)
; ch = selected cylinder (1 based)
; dh = selected track (0 based)
; cl = selected sector (1 based)
; al = sector read count (1 based)
; bx = address of the data to be stored that is read from drive
disk_load_16bit:
    pusha

    mov ah, 0x02
    int 0x13

    cmp al, 0
    je disk_error

    popa
    ret

disk_error:
    mov bx, disk_error_string
    call print_string_16_bit
    jmp $

disk_error_string:
    db "Failed to read from disk", 0