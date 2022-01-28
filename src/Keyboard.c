#include "System.h"
//ASCII correspondiente al scancode (teclado en español)
unsigned char kb_es[128] =
{
0 /*Error*/, 
27 /*ESC*/, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 39 /* ' */, 0 /*?*/, '\b' /*Retroceso*/,
'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0 /*grave*/, '+', '\n' /*ENTER*/, 0 /*CTRL*/,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 0 /*ñ*/, '{', '|', 0 /*LSHIFT*/, '}',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 0 /*RSHIFT*/,
'*', 0 /*ALT*/, ' ', 0 /*BLOQ MAYUS*/, 
/*  F1 F2 F3 F4 F5 F6 F7 F8 F9 F10  */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0 /*BLOQ NUM*/, 0 /*BLOQ DESPL*/, 
0 /*INICO / NUM 7*/, 0 /*UP / NUM 8*/, 0 /*RE PAG / NUM 9*/,
'-',
0 /*LEFT*/, 0 /*NUM 5*/, 0 /*RIGHT / NUM 6*/,
'+',
0 /*FIN / NUM 1*/, 0 /*DOWN / NUM 2*/, 0/*AV PAG / NUM 3*/,
0 /*INS / NUM 0*/, 0 /*SUPR / .*/,
0 /*IMPR PANT*/, 0 /*Sin definir*/, '<',
/*  F11 F12 Sin definir Windows APP*/
    0,  0,  0, 0,       0, 0,   0
/*El resto de las teclas estan por definirse*/
};

void keyboard_handler(struct regs *r)
{
    unsigned char scancode;
    asm("in $0x60, %%AL":"=r"(scancode)); 
    
    //Se acaba de soltar una tecla
    if(scancode & 0x80) 
    {
        //Checar si SHIFT, ALT, CTRL fueron soltadas
    }
    //Una tecla fue apretada
    else
    {
        //TO DO: implementacion extra con SHIFT, ALT, CTRL, etc.
        printv("%c", kb_es[scancode]);
    }
}

void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}