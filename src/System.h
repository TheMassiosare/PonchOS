#define DISABLE_CURSOR 0b00100000 
#define ENABLE_NORMAL_CURSOR 0b00001101 

struct regs
{
    unsigned int gs, fs, es, ds;                         
    unsigned int edi, eso, ebp, esp, ebx, edx, ecx, eax; 
    unsigned int int_no, err_code;                       
    unsigned int eip, cs, eflags, useresp, ss;           
};

void printv(const char* str, ...);
void IDT_install();
void idt_set_gate(unsigned char n, unsigned long base, unsigned short selector, unsigned char flags);
void ISRs_install();
void IRQ_install();
void irq_install_handler(int irq, void (*handler)(struct regs *r));
void irq_uninstall_handler(int irq);
void timer_install();
void timer_wait(int ticks);
void keyboard_install();
unsigned long strlen(char* str);
void outportb(unsigned short port, unsigned char val);
unsigned char inportb(unsigned char port);
extern void enable_disable_cursor(unsigned char E_D);
extern void update_cursor(int x, int y);

//x=get_cursor_position()%80, y=get_cursor_position()/80
extern unsigned short get_cursor_position();
char* int_to_str(int n, char* str_out, unsigned char base);
char* reverse_str(char* str);
void* memset(void* dest, int val, unsigned long len);
void* memcopy(void* from, void* to, unsigned long size);
void mem_init();
void* kmalloc(unsigned long size);
void kfree(void* ptr);
void display_mem();

