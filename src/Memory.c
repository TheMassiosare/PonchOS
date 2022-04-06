#include "System.h"
#define MEM_START 0x10000
#define MEM_SIZE 1024

//1KiB disponible
//1 ocupado, 0 libre
char b_free[MEM_START];
unsigned char first_free;
void mem_init()
{
    first_free = 0;
    unsigned short i = 0;
    do
    {
        b_free[i] = 0;
    } while (++i < MEM_SIZE);
    
}

void find_first_free()
{
    unsigned short i = 0;
    do
    {
        if((b_free[i]|b_free[i+1]|b_free[i+2]) == 0)
        {
            first_free = i+1;
            return;
        }
    } while (++i < MEM_SIZE);
    
}

void display_mem()
{
    unsigned short i = 0;
    do
    {
        printv("%c", b_free[i]+'0');
    } while (++i < MEM_SIZE);
    
}

//Regresa 0 si falla
void* kmalloc(unsigned long size)
{
    unsigned long space = 0;
    for(unsigned short i = first_free; i < MEM_SIZE; i++)
    {
        if(b_free[i] == 1)
        {
            space = 0;
            continue;
        }

        if(++space == size && b_free[i+1] == 0)
        {
            for(unsigned short j = i-size+1; j <= i; j++)
                b_free[j] = 1;
            find_first_free();
            return (void*)(MEM_START+i-size+1);
        }
    }

    return 0;
}

void kfree(void* ptr)
{
    unsigned long len = 0;
    unsigned long i = (unsigned long)ptr-(unsigned long)MEM_START;
    while(b_free[i])
    {
        len++; 
        b_free[i++] = 0;
    }
    memset(ptr, '\0', len);
    find_first_free();
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