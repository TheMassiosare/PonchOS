#include "System.h"

extern void main()
{
    IDT_install();
    IRQ_install();
    enable_disable_cursor(ENABLE_NORMAL_CURSOR);    
    printv("ASTRALOPITecuS%i", 10);
}