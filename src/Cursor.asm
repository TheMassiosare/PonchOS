global enable_disable_cursor
global update_cursor
global get_cursor_position

enable_disable_cursor:
    push ebp         
    mov ebp, esp     
 
	mov dx, 0x3D4
	mov al, 0xA	; low cursor shape register
	out dx, al
 
    
	inc dx
    ;Bits 6-7 sin usar. Bit 5 set desactiva el cursor. Bits 0-4 controlan la altura del cursor
	mov al, 0	
    mov ebx, [ebp+8]
    or al, bl
	out dx, al
                         
    leave           
    ret

update_cursor:
    push ebp         
    mov ebp, esp 

    VGA_Width equ 80
        
    mov ebx, dword [ebp+8]  ;x
    mov eax, dword [ebp+12] ;y
    mov dl, VGA_Width
    mul dl
    add bx, ax

    mov dx, 0x3D4
    mov al, 0x0F
    out dx, al

    inc dx
    mov al, bl
    out dx, al

	; outb(0x3D4, 0x0E);
    dec dx
    mov al, 0x0E
    out dx, al

    inc dx
    mov al, bh
    out dx, al
    
    leave 
    ret

get_cursor_position:
    push ebp         
    mov ebp, esp 

    xor bx, bx

    mov dx, 0x3D4
    mov al, 0x0F
    out dx, al

    inc dx
    in al, dx
    or bl, al

    dec dx
    mov al, 0x0E
    out dx, al

    inc dx
    in al, dx
    shl ax, 0x8
    or ax, bx

    leave 
    ret
