#include "System.h"

void timer_phase(int hz)
{
    int divisor = 1193180/hz;
    outportb(0x43, 0x36); //Configurar registro de comandos para el timer
    outportb(0x40, divisor & 0xFF); //LSByte del divisor
    outportb(0x40, divisor >> 8);   //MSByte del divisor
}

int timer_ticks = 0;

void timer_handler(struct regs* r)
{
    timer_ticks++;
    if(timer_ticks%100 == 0)
        printv("Paso un segundo\n");
}

void timer_install()
{
    timer_phase(100);
    irq_install_handler(0, timer_handler);
}

void timer_wait(int ticks)
{
    unsigned long eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}