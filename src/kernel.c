#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "system.h"
//bayad compiler i386 bashe

size_t g_sStack[4094];  

void kernel_main()
{
	terminal_initialize();
    gdt_init();
    idt_init();
    pic_init();
    __asm__ __volatile__ ("sti");

	while(true)
	{
        /*
        //iin baraye iine kernel hamiish e ejra bashe :D
        */
	}
}
