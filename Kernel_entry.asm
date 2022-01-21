[bits 32]

global idt_load
[extern idtp]

[extern main]
call main

jmp $

idt_load: 
    lidt [idtp]
    ret