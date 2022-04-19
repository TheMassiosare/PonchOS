#include "System.h"

extern void main()
{
    change_output_color(LIGHT_GREY, BLACK);
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

    printv("Starting\n\n");
    {
        change_output_color(LIGHT_GREY | INTENSITY, BLACK);
        unsigned short pos0 = get_cursor_position();
        char PonchOS[] =
        {
"\
   /$$$$$$$                                /$$        /$$$$$$   /$$$$$$   /$$\n\
  | $$__  $$                              | $$       /$$__  $$ /$$__  $$ | $$\n\
  | $$  \\ $$  /$$$$$$  /$$$$$$$   /$$$$$$$| $$$$$$$ | $$  \\ $$| $$  \\__/ | $$\n\
  | $$$$$$$/ /$$__  $$| $$__  $$ /$$_____/| $$__  $$| $$  | $$|  $$$$$$  | $$\n\
  | $$____/ | $$  \\ $$| $$  \\ $$| $$      | $$  \\ $$| $$  | $$ \\____  $$ |__/\n\
  | $$      | $$  | $$| $$  | $$| $$      | $$  | $$| $$  | $$ /$$  \\ $$    \n\
  | $$      |  $$$$$$/| $$  | $$|  $$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/  /$$\n\
  |__/       \\______/ |__/  |__/ \\_______/|__/  |__/ \\______/  \\______/  |__/\n"
        };
        printv(PonchOS);
        unsigned short pos1 = get_cursor_position();
        color_rectangle(0, 79, pos0/80-1, pos1/80, CYAN);
    }
    
    change_output_color(LIGHT_GREY, BLACK);
}
