[org 0x7c00]

[bits 16]

mov [BOOT_DRIVE], dl

mov bp, 0x7c00
mov sp, bp

mov dl, [BOOT_DRIVE] ; Read data from boot drive
mov ch, 0 ; Select cylinder 0
mov dh, 0 ; Select track 0
mov cl, 2 ; Select sector 2 (because our boot sector is in first sector)
mov al, 40 ; Read 40 sectors

mov bx, KERNEL_START ; Store data in address => 0x0000(ES) : 0x7e00(BX) = 0x7e00
call disk_load_16bit

call switch_to_pm

jmp $

%include "16bit/inout_16bit.asm"

%include "x86/switch_to_pm.asm"
%include "x86/inout_x86.asm"

BEGIN_PM:
    call KERNEL_START

    jmp $

BOOT_DRIVE:
    db 0

KERNEL_START equ 0x7e00

times 510 -($-$$) db 0

dw 0xaa55