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
    
    ;SetCoords (VGA_WIDTH*y +x):
    mov ebx, [ebp+8]  ;x
    mov eax, [ebp+12] ;y
    mov dl, VGA_Width
    mul dl
    add bx, ax

    ;SetOffset:    
    mov dx, 0x03D4
    mov al, 0x0F
    out dx, al

    inc dl
    mov al, bl
    out dx, al

    dec dl
    mov al, 0x0E
    out dx, al

    inc dl
    mov al, bh
    out dx, al
    
    leave 
    ret

get_cursor_position:
    push ebp
    mov ebp, esp
    sub esp, 0d32

    ;pos = 0
    xor ax, ax

    ;outb(0x3D4, 0x0F)
    mov dx, 0x3D4
    mov al, 0x0F
    out dx, al

    ;pos |= inb(0x3D5)
    inc dl
    in al, dx
    mov bl, al
    
    ;outb(0x3D4, 0x0E)
    dec dl
    mov al, 0x0E
    out dx, al

    ;pos |= ((uint16_t)inb(0x3D5)) << 8;
    inc dl
    in al, dx 
    shl ax, 0x8
    xor bh, bh
    or ax, bx
    xor edx, edx

;y = pos / VGA_WIDTH; x = pos % VGA_WIDTH;. 
    mov cx, VGA_Width
    div cx
    shl edx, 0d16
    or eax, edx

    leave 
    ret