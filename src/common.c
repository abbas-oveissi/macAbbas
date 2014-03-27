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


char b[] = {'0','1','2','3','4','5','6','7','8','9'};
char * intToStr(unsigned int ptr)
{
	char buff[33],temp[33];
	int i=31;
	buff[32]='\0';
	while(ptr!=0)
	{
		char ch=b[ptr%10];
		ptr=ptr/10;
		buff[i]=ch;
		i=i-1;
	}
	i++;
	for(int j=0;;++i,j++)
	{
		if(buff[i]=='\0')
		{
			temp[j]='\0';
			break;
		}
		temp[j]=buff[i];
	}
	return (char *)temp;
}

