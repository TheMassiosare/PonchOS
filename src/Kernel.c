#include "System.h"

extern void main()
{
    IDT_install();
    IRQ_install();
    timer_install();
    enable_disable_cursor(ENABLE_NORMAL_CURSOR);  
    printv("Starting PonchOS!\n");
}