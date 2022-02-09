#include "System.h"
#include <stdarg.h>
#include <stddef.h>

#define TAB_SPACE 4

//typedef unsigned int size_t;
void putstr(const char *str, size_t n) {
    if (n > 0) {
        unsigned int tmp_cursor = get_cursor_position();
        int x = (unsigned short)(tmp_cursor >> 16);
        int y = (unsigned short)tmp_cursor;
        size_t i;
    
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
            default:
                *(char *)(0xb8000 + (y * 80 + x) * 2) = str[i];
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
            str = ptr += 2;
            switch (ptr[-1]) {
            case 'c':
                buffer[0] = (char)va_arg(list_ptr, int);
                putstr(buffer, 1);
                break;
            case 's':
                p = va_arg(list_ptr, char *);
                putstr(p, strlen(p));
                break;
            case 'i':
            case 'd':                
                int_to_str(va_arg(list_ptr, int), buffer, 10);
                putstr(buffer, strlen(buffer));
                break;
            case '%':
                str -= 1; // include the second % in the next span
                break;
            default:
                str -= 2; // include format in the next span
                break;
            }
        }     
    }
    putstr(str, ptr - str);
    va_end(list_ptr);
}

void* memset(void* dest, int val, unsigned long len)
{
    unsigned char* ptr = dest;
    while(len--)
        *ptr++ = val;
    return dest;
}

void* memcopy(void* from, void* to, unsigned long size)
{
    unsigned char *f_ptr = from, *t_ptr = to;
    for(;*f_ptr != 0; f_ptr++, t_ptr++)
        *t_ptr = *f_ptr;
    *t_ptr = 0;
    return to;
}

unsigned long strlen(char* str)
{
    unsigned long len = 0;
    char* ptr = str;
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

