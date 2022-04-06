#include "System.h"

extern void main()
{
    printv("Installing IDT...");
    IDT_install();
    printv("Success\n");
    printv("Installing IRQs...");
    IRQ_install();    
    asm volatile ("sti"); //Permite que haya IRQs 
    printv("Success\n");
    printv("Installing CPU timer...");
    timer_install();
    printv("Success\n");
    printv("Installing keyboard drivers...");
    keyboard_install();
    printv("Success\n");
    enable_disable_cursor(ENABLE_NORMAL_CURSOR); 
    printv("Starting memory...");
    mem_init();
    printv("Success\n");

    printv("Starting PonchOS!...\n");

}
