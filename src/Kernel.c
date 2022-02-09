#include "System.h"

extern void main()
{
    IDT_install();
    IRQ_install();
    asm volatile ("sti"); //Permite que haya IRQs 
    timer_install();
    keyboard_install();
    enable_disable_cursor(ENABLE_NORMAL_CURSOR);  
    
    //Sigue habiendo problemas al imprimir chars, pues se imprime el '\0' tambien

    printv("Starting PonchOS!\n");
    printv("Testtesttesttest");
    printv("Testtesttesttest");
    printv("Testtesttesttest");
    printv("Testtesttesttest");
    printv("Testtesttesttest");
    printv("Testtesttesttest");

    char str[] = "Hello\0";
    char c = 'C';
    //printv("%s", str);
    printv("%c", c);
    printv("%i", 123);
}