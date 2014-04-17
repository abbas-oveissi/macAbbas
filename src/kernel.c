#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"

size_t g_sStack[4094];  

void kernel_main()
{

	terminal_initialize();
    gdt_init();
    idt_init();
    pic_init();
    __asm__ __volatile__ ("sti");
    terminal_writestring("          +--------------------------------------------------------+\n");
    terminal_writestring("          |                 MacAbbas Operating System              |\n");
    terminal_writestring("          +--------------------------------------------------------+\n\n\n");   

	while(true)
	{
        /*
        //iin baraye iine kernel hamiish e ejra bashe :D
        */
	}
}
