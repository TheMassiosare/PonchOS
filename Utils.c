#include "System.h"
#include <stdarg.h>

#define TAB_SPACE 4

struct coordinates
{
    unsigned short x, y;
}__attribute__((packed));

static struct coordinates cursor_position;

void printv(char* str, ...)
{
    unsigned int tmp_cursor = get_cursor_position();
    cursor_position.x = (unsigned short)(tmp_cursor >> 16);
    cursor_position.y = (unsigned short)tmp_cursor;
    char buffer[12];
    memset(buffer, '\0', 12);

    va_list list_ptr;
    va_start(list_ptr, str);
    
    unsigned int i = 0;
    for(char* ptr = str; *ptr != '\0'; ptr++)
    {    
        switch (*ptr)
        {
        case '%':
            cursor_position.y += (cursor_position.x+i)/80;
            cursor_position.x = (cursor_position.x+i)%80;            
            update_cursor(cursor_position.x, cursor_position.y);
            i = 0;
            switch (*(ptr+1))
            {
            case 'c':
                buffer[0] = (char)va_arg(list_ptr, int);
                buffer[1] = '\0';
                printv(buffer);
                ptr++;
                continue;
            case 's':
                printv(va_arg(list_ptr, char*));
                ptr++;
                continue;
            //Tiene un comportamiento extraño cada que hay menos de 7 caracteres
            //antes del %   
            case 'i':
            case 'd':                
                int_to_str(va_arg(list_ptr, int), buffer, 10);                
                printv(buffer);
                ptr++;
                continue;         
            default:     
                *(char*)(0xb8000+(cursor_position.x+i+cursor_position.y*80)*2)  = *ptr;
                i++;       
                break;
            }  
            break;          
        case '\n':
            i = 0;
            cursor_position.x = 0;
            cursor_position.y++;
            break;
        case '\t':
            cursor_position.y += (cursor_position.x+i)/80;
            cursor_position.x = (cursor_position.x+i)%80;                    
            update_cursor(cursor_position.x, cursor_position.y);
            i = 0;
            cursor_position.x += TAB_SPACE-cursor_position.x%TAB_SPACE-1;        
            break;
        default:
            *(char*)(0xb8000+(cursor_position.x+i+cursor_position.y*80)*2)  = *ptr;
            i++;
            break;
        }     
    }

    va_end(list_ptr);

    cursor_position.y += (cursor_position.x+i)/80;
    cursor_position.x = (cursor_position.x+i)%80;  
    update_cursor(cursor_position.x, cursor_position.y);
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

void outportb(unsigned short port, unsigned char val)
{
    asm("mov %0, %%dx"::"r"(port):); 
    asm("mov %0, %%al"::"r"(val):); 
    asm("out %al, %dx");
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

