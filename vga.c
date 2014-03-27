#if !defined(__cplusplus)
#include <stdbool.h> /* C doesn't have booleans by default. */
#endif
#include <stddef.h>
#include <stdint.h>
#include "system.h"



/* Hardware text mode color constants. */
enum vga_color
{
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN = 14,
	COLOR_WHITE = 15,
};
 



uint8_t make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color)
{
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t strlen(const char* str)
{
	size_t ret = 0;
	while ( str[ret] != 0 )
		ret++;
	return ret;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 


void writeForVGA(uint8_t index,uint8_t value)
{
    outportb(0x3D4,index);
    outportb(0x3D5,value);
}


void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_WHITE, COLOR_BLUE);
	terminal_buffer = (uint16_t*) 0xB8000;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ )
	{
		for ( size_t x = 0; x < VGA_WIDTH; x++ )
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}


}
 


uint8_t keyboard_read_scanCode()
{
	return inportb(0X60);
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}
 
void backSpaceKey()
{
	if ( --terminal_column == -1 )
	{
		terminal_row--;
		terminal_column = VGA_WIDTH-1;
		if ( terminal_row == -1 )
		{
			terminal_row = 0;
		}
	}
	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
	
	size_t cursorLocation = terminal_row* VGA_WIDTH + terminal_column;
    writeForVGA(0x0E,cursorLocation >> 8);
    writeForVGA(0x0F,cursorLocation);
}

void terminal_putchar(char c)
{
	if(c=='\n')
	{
		terminal_column=0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_row--;
			scroll_down();
			//terminal_row = 0;
		}
		size_t cursorLocation = terminal_row* VGA_WIDTH + terminal_column;
    	writeForVGA(0x0E,cursorLocation >> 8);
    	writeForVGA(0x0F,cursorLocation);
		return;
	}
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if ( ++terminal_column == VGA_WIDTH )
	{
		terminal_column = 0;
		if ( ++terminal_row == VGA_HEIGHT )
		{
			terminal_row--;
			scroll_down();
			//terminal_row = 0;
		}
	}
	size_t cursorLocation = terminal_row* VGA_WIDTH + terminal_column;
    writeForVGA(0x0E,cursorLocation >> 8);
    writeForVGA(0x0F,cursorLocation);
}
 
void scroll_down()
{
	for(int i=1;i<VGA_HEIGHT;i++)
	{
		for(int j=0;j<VGA_WIDTH;j++)
		{
			size_t oldLocation = i* VGA_WIDTH + j;
			size_t newLocation = (i-1)* VGA_WIDTH + j;
			terminal_buffer[newLocation] =terminal_buffer[oldLocation];
		}
	}

	for(int j=0;j<VGA_WIDTH;j++)
	{
		size_t index = terminal_row* VGA_WIDTH + j;
		terminal_buffer[index]=make_vgaentry(' ', terminal_color);
	}

}

void terminal_writestring(const char* data)
{
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		terminal_putchar(data[i]);
}

char bchars[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char * ptrToStr(unsigned int ptr,int base)
{
	char buff[33],temp[33];
	int i=31;
	buff[32]='\0';
	while(ptr!=0)
	{
		char ch=bchars[ptr%base];
		ptr=ptr/base;
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

