#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"


unsigned char inportb (uint16_t _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (uint16_t _port, uint8_t _data)
{
     __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void *memset(void *s, int c, size_t n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}