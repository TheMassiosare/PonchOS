#include "System.h"
#include <stdarg.h>

#define TAB_SPACE 4

enum colors FOREGROUND_COLOR = LIGHT_GREY | INTENSITY;
enum colors BACKGROUND_COLOR = BLUE;

void change_output_color(enum colors f_color, enum colors b_color)
{
    FOREGROUND_COLOR = f_color; 
    BACKGROUND_COLOR = b_color;
}

static inline unsigned short color_text(unsigned char chr, enum colors f_color, enum colors b_color)
{
    return (unsigned short) chr | (unsigned short) (f_color | b_color << 4) << 8;
}

//Creditos por putstr y printv a chqrlie en StackOverflow
void putstr(const char *str, unsigned long n) {
    if (n > 0) {
        unsigned short tmp_cursor = get_cursor_position();
        unsigned short x = tmp_cursor%80;
        unsigned short y = tmp_cursor/80;
        unsigned long i;
    
        for (i = 0; i < n; i++) {
            switch (str[i]) {
            case '\n':
                y += x / 80 + 1;
                x = 0;
                break;
            case '\r':
                y += x / 80;
                x = 0;
                break;
            case '\t':
                x = (x + TAB_SPACE) / TAB_SPACE * TAB_SPACE;        
                y += x / 80;
                x %= 80;
                break;
            case '\b':
                if(x%80 != 0)
                {
                    x--; 
                    *(char *)(0xb8000 + (y * 80 + x) * 2) = ' ';    
                }                
                break;
            default:
                *(short*)(0xb8000 + (y * 80 + x) * 2) = color_text(str[i], FOREGROUND_COLOR, BACKGROUND_COLOR);
                x++;
                break;
            }
        }     
        update_cursor(x, y);
    }
}

void printv(const char *str, ...) {
    char buffer[32];
    char *p;
    const char *ptr;
    va_list list_ptr;
    va_start(list_ptr, str);

    for (ptr = str; *ptr != '\0'; ptr++) {
        if (*ptr == '%' && ptr[1] != '\0') {
            putstr(str, ptr - str);
            str = ptr;
            ptr++;
            switch (*ptr) {
            case 'c':
                buffer[0] = (char)va_arg(list_ptr, int);
                putstr(buffer, 1);
                str += 2; // skip the format
                break;
            case 's':
                p = va_arg(list_ptr, char *);
                putstr(p, strlen(p));
                str += 2; // skip the format
                break;
            case 'i':
            case 'd':                
                int_to_str(va_arg(list_ptr, int), buffer, 10);
                putstr(buffer, strlen(buffer));
                str += 2; // skip the format
                break;
            case '%':
                str += 1; // skip the initial %
                break;
            }
        }     
    }
    putstr(str, ptr - str);
    va_end(list_ptr);
}

void color_line(unsigned char line_no, enum colors b_color)
{
    for(unsigned char x = 0; x < 80; x++)
        *(short*)(0xb8000 + (line_no * 80 + x) * 2) = color_text(*(char*)(0xb8000 + (line_no * 80 + x) * 2), FOREGROUND_COLOR, b_color);    
}

void color_column(unsigned char column_no, enum colors b_color)
{
    for(unsigned char y = 0; y < 25; y++)
        *(short*)(0xb8000 + (y * 80 + column_no) * 2) = color_text(*(char*)(0xb8000 + (y * 80 + column_no) * 2), FOREGROUND_COLOR, b_color);    
}

void color_rectangle(unsigned char x0, unsigned char x1, unsigned char y0, unsigned y1, enum colors b_color)
{
    if(x0 > x1 || y0 > y1) 
        return;
    
    unsigned char start_x = x0;
    for(; y0 <= y1; y0++)
    {
        for(; x0 <= x1; x0++)
            *(short*)(0xb8000 + (y0 * 80 + x0) * 2) = color_text(*(char*)(0xb8000 + (y0 * 80 + x0) * 2), FOREGROUND_COLOR, b_color);    
        x0 = start_x;
    }

}

unsigned long strlen(char* str)
{
    unsigned long len = 0;
    const char* ptr = str;
    while(*(ptr++))
        len++;
    return len;
}

void outportb(unsigned short port, unsigned char val)
{
    asm("mov %0, %%dx"::"r"(port):); 
    asm("mov %0, %%al"::"r"(val):); 
    asm("out %al, %dx");
}

unsigned char inportb(unsigned char port)
{
    unsigned char read;
    asm("mov %0, %%dx"::"r"((unsigned short)port));
    asm ("inb %%dx, %%al":"=r"(read)::"%dx"); 
    return read;
}

char* int_to_str(int n, char* str_out, unsigned char base)
{
    char* str = str_out;
    int digit, sign = 0;
    if(n < 0)
    {
        sign = 1; 
        n *= -1;
    }
    while(n)
    {
        digit = n%base;
        *str = (digit > 9) ? ('A'+digit-10) : ('0'+digit);        
        n /= base;
        str++;
    }
    if(sign) 
        *str++ = '-';
    *str = '\0';
    reverse_str(str_out);

    return  str_out;
}

char* reverse_str(char* str)
{
    if(!str)
        return 0;
    unsigned long len = 0;
    for(char* ptr = str; *ptr != 0; ptr++)
        len++;

    char tmp_c;
    for(int i = 0, j = len-1; i < j; i++, j--)
    {
        tmp_c = str[i];
        str[i] = str[j];
        str[j] = tmp_c;
    }
    return str;
}
