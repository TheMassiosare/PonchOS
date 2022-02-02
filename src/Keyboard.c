#include "System.h"
//ASCII correspondiente al scancode (teclado en español)
unsigned char kb_es[512] =
{
/* Teclas normales */    
0 /*Error*/, 
27 /*ESC*/, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 39 /* ' */, 168 /*¿*/, '\b' /*Retroceso*/,
'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 0 /*grave*/, '+', '\n' /*ENTER*/, 178 /*CTRL*/,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 164 /*ñ*/, '{', '|', 176 /*LSHIFT*/, '}',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', 176 /*RSHIFT*/,
'*', 177 /*ALT*/, ' ', 217 /*BLOQ MAYUS*/, 
/*  F1 F2 F3 F4 F5 F6 F7 F8 F9 F10  */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
218 /*BLOQ NUM*/, 219 /*BLOQ DESPL*/, 
0 /*INICO / NUM 7*/, 0 /*UP / NUM 8*/, 0 /*RE PAG / NUM 9*/,
'-',
0 /*LEFT*/, 0 /*NUM 5*/, 0 /*RIGHT / NUM 6*/,
'+',
0 /*FIN / NUM 1*/, 0 /*DOWN / NUM 2*/, 0/*AV PAG / NUM 3*/,
0 /*INS / NUM 0*/, 0 /*SUPR / .*/,
0 /*IMPR PANT*/, 0 /*Sin definir*/, '<',
/*  F11 F12 Sin definir Windows APP*/
    0,  0,  0, 0,       0, 0,   0,
/*El resto de las teclas estan por definirse*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,

/* SHIFT activado */
0 /*Error*/, 
27 /*ESC*/, '!', 34 /* " */, '#', '$', '%', '&', '/', '(', ')', '=', '?', 173 /*¡*/, '\b' /*Retroceso*/,
'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',  0 /*dieresis*/, '*', '\n' /*ENTER*/, 178 /*CTRL*/,
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 165 /*Ñ*/, '[', 167 /*°*/, 176 /*LSHIFT*/, ']',
'Z', 'X', 'C', 'V', 'B', 'N', 'M', ';', ':', '_', 176 /*RSHIFT*/,
'*', 177 /*ALT*/, ' ', 217 /*BLOQ MAYUS*/, 
/*  F1 F2 F3 F4 F5 F6 F7 F8 F9 F10  */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
218 /*BLOQ NUM*/, 219 /*BLOQ DESPL*/, 
0 /*INICO / NUM 7*/, 0 /*UP / NUM 8*/, 0 /*RE PAG / NUM 9*/,
'-',
0 /*LEFT*/, 0 /*NUM 5*/, 0 /*RIGHT / NUM 6*/,
'+',
0 /*FIN / NUM 1*/, 0 /*DOWN / NUM 2*/, 0/*AV PAG / NUM 3*/,
0 /*INS / NUM 0*/, 0 /*SUPR / .*/,
0 /*IMPR PANT*/, 0 /*Sin definir*/, '<',
/*  F11 F12 Sin definir Windows APP*/
    0,  0,  0, 0,       0, 0,   0,
/*El resto de las teclas estan por definirse*/
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

/*
    *bit 0: SHIFT ON
    *bit 1: ALT ON
    *bit 2: CTRL ON
*/

unsigned char status = 0;

void keyboard_handler(struct regs *r)
{
    unsigned char scancode = inportb(0x60);
    
    //Se acaba de soltar una tecla
    if(scancode & 0x80) 
    {
        switch (kb_es[scancode&0x7F])
        {
        case 176: /* SHIFT */
            status &= 0b110;
            break;
        case 177: /* ALT */
            status &= 0b101;
            break;
        case 178: /* CTRL */
            status &= 0b011;
            break;            
        default:
            break;
        }
    }

    //Una tecla fue apretada
    else
    {
        //TO DO: implementacion extra con SHIFT, ALT, CTRL, etc.
        switch (kb_es[scancode])
        {       
        case 176: /* SHIFT */
            status |= 0b001;
            break;
        case 177: /* ALT */
            status |= 0b010;
            break;
        case 178: /* CTRL */
            status |= 0b100;
            break;
        case 217: /* CAPS LOCK */     
            while(1)
            {
                if((inportb(0x64)&2) == 0)
                    break;
            }
            outportb(0x60, 0xED);
            outportb(0x60, 0b100);
            break;  
        case 218: /* BLOQ NUM */
            while(1)
            {
                if((inportb(0x64)&2) == 0)
                    break;
            }
            outportb(0x60, 0xED);
            outportb(0x60, 0b010);
            break;
        case 219: /* BLOQ DESPL */
            while(1)
            {
                if((inportb(0x64)&2) == 0)
                    break;
            }
            outportb(0x60, 0xED);
            outportb(0x60, 0b001);
            break;
        default:
            scancode += 128*(status&0x1+(status&0x2)/2+(status&0x3)/4);
            printv("%c", kb_es[scancode]);
            break;
        }
    }
}

void keyboard_install()
{
    irq_install_handler(1, keyboard_handler);
}