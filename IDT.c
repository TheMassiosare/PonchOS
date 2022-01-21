#include "System.h"

struct IDT_entry
{
    unsigned short base_low, selector;
    unsigned char zeros, flags;
    unsigned short base_high;
}__attribute__ ((packed));

struct IDT_ptr
{
    unsigned short limit;
    unsigned int base;
}__attribute__((packed));

struct IDT_entry idt[256];
struct IDT_ptr idtp;

extern void idt_load();

//Configurar la n-esima entrada del IDT
void idt_set_gate(unsigned char n, unsigned long base, unsigned short selector, unsigned char flags)
{
    idt[n].base_low = (unsigned short)base;
    idt[n].selector = selector;
    idt[n].zeros = 0;
    idt[n].flags = flags;
    idt[n].base_high = (unsigned short)(base >> 16);
}

void IDT_install()
{
    idtp.limit = (sizeof(struct IDT_entry)*256)-1;
    idtp.base = &idt;

    memset(&idt, 0, sizeof(struct IDT_entry)*256);

    ISRs_install();

    idt_load();
}