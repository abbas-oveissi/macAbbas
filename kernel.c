#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"
//bayad compiler i386 bashe

size_t g_sStack[4094];  


    // none isr
void keyboard_handler(registers_t regs)
{
    uint8_t status= inportb(0x64);
    if(status&0x1==1)
    {
        uint8_t a= keyboard_read_scanCode();
        if(a==0x1e)
        {
            terminal_putchar('a');
        }
        else if(a==0x1f)
        {
            terminal_putchar('s');
        }
    }
}
    

void kernel_main()
{
	terminal_initialize();

    //terminal_writestring(ptrToString(&gp,16));
    //terminal_writestring("\n");
    gdt_init();
    idt_init();
    pic_init();

    register_isr(33,keyboard_handler);
    __asm__ __volatile__ ("sti");

    //terminal_writestring("a\n");
	while(true)
	{
        /*
		uint8_t status= inportb(0x64);
		if(status&0x1==1)
		{
			uint8_t a= keyboard_read_scanCode();
			if(a==0x1e)
			{
				terminal_writestring("a");
			}
			else if(a==0x1f)
			{
				terminal_writestring("s");
			}
		}
        */
	}
	terminal_writestring("be os man khosh oomadid :D!\nMac Abbas OS");
}
