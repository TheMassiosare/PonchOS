[org 0x7c00]
KERNEL_LOCATION equ 0x1000
mov [BOOT_DISK], dl

;Configurar registros 
mov bp, 0x800
mov sp, bp
xor ax, ax
mov ds, ax
mov es, ax

;Leer el Kernel del disco
mov ah, 02
mov al, 20
mov bx, KERNEL_LOCATION
mov ch, 0
mov cl, 2
mov dh, 0
mov dl, [BOOT_DISK]
int 0x13
jnc success
cmp ah, 0
jne success
;Mensaje de error
mov ah, 0x0e
mov al, 'E'
int 0x10
success:
;Modo de texto (limpia pantalla)
mov ax, 0x3
int 0x10

%include "GDT.asm"

[bits 32]
start_protected_mode:
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    jmp KERNEL_LOCATION
jmp $

BOOT_DISK db 0
times 510-($-$$) db 0
dw 0xaa55