#include "system.h"
#include <stdint.h>
//================================
// local variables
//================================

// interrupt handlers
extern isr_t interrupt_handlers[MAX_INTERRUPTS];

//================================
// functions
//================================


// none isr
void isr_none(registers_t regs)
{
}

// registerInterruptHandler
void register_isr( uint8_t n, isr_t handler )
{
    interrupt_handlers[n] = handler;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else if(regs.int_no==13)
    {
        terminal_writestring("Recieved interrupt: ");
        terminal_writestring(intToStr(regs.int_no));
        terminal_writestring("\n");
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{   
    // sending EOI (end of interrup) command to the pic
    if ( regs.int_no >= 40 )
    {
        outportb(0xA0, 0x20);
    }
    outportb(0x20, 0x20);
    
    // call cusomized handler if found
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        if(regs.int_no!=32)
        {
            terminal_writestring("Recieved IRQ: ");
            terminal_writestring(ptrToStr(regs.int_no,10));
            terminal_writestring("\n");
        }

    }


}



#define PIC1_REG_COMMAND    0x20
#define PIC1_REG_DATA       0x21
#define PIC2_REG_COMMAND    0xA0
#define PIC2_REG_DATA       0xA1

#define MASTER_PIC_BASE     0x20
#define SLAVE_PIC_BASE      0x28
#define IDT_DESC_BIT16    0x06  //00000110
#define IDT_DESC_BIT32    0x0E  //00001110

#define IDT_DESC_RING0    0x0   //00000000
#define IDT_DESC_RING1    0x40  //01000000
#define IDT_DESC_RING2    0x20  //00100000
#define IDT_DESC_RING3    0x60  //01100000

#define IDT_DESC_PRESENT  0x80  //10000000
extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

 




char * scanCodeKeyboard[128] = {
    "Err",
    "Esc",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "_",
    "=",
    "BackSpace",
    "Tab",
    "q",
    "w",
    "e",
    "r",
    "t",
    "y",
    "u",
    "i",
    "o",
    "p",
    "[",
    "]",
    "Enter",
    "L ctr",
    "a",
    "s",
    "d",
    "f",
    "g",
    "h",
    "j",
    "k",
    "l",
    ";",
    "\'",
    "~",
    "L shift",
    "\\",
    "z", 
    "x",
    "c",
    "v",
    "b",
    "n",
    "m",
    ",",
    ".",
    "/",
    "#",
    "#",
    "#",
    " " // R shift - bikhi - L alt
};
char * shifScanCodeKeyboard[128] = {
    "Err",
    "Esc",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "0",
    "_",
    "=",
    "BackSpace",
    "Tab",
    "q",
    "w",
    "e",
    "r",
    "t",
    "y",
    "u",
    "i",
    "o",
    "p",
    "[",
    "]",
    "Enter",
    "L ctr",
    "A",
    "S",
    "D",
    "F",
    "G",
    "H",
    "J",
    "K",
    "L",
    ";",
    "\'",
    "~",
    "L shift",
    "\\",
    "z", 
    "x",
    "c",
    "v",
    "b",
    "n",
    "m",
    ",",
    ".",
    "/",
    "#",
    "#",
    "#",
    " " // R shift - bikhi - L alt
};

int shift=0;
extern void backSpaceKey();



void keyboard_handler(registers_t regs)
{
    uint8_t status= inportb(0x64);
    if(status&0x1==1)
    {
        uint8_t a= keyboard_read_scanCode();
        //terminal_writestring(ptrToStr(a,16));
        //terminal_writestring("\n");
        if(a==0x2A)
        {
            shift=1;
            return;
        }
        else if(a==0xAA)
        {
            shift=0;
            return;

        }

        if(a==28)
        {
            terminal_writestring("\n");
            return;
        }
        //if(a==14)
        //{
        //    //backSpaceKey();
        //    return;
        //}
        if(a<128&&shift==0)
        {
            terminal_writestring(intToStr(shift));
            terminal_writestring(scanCodeKeyboard[a]);
            return;
        }
        if(a<128&&shift==1)
        {
            terminal_writestring(intToStr(shift));
            terminal_writestring(shifScanCodeKeyboard[a]);
            return;
        }
         //terminal_writestring("------\n");

        // terminal_writestring("------\n");

    }
}

// pic_init
void pic_init( )
{
    // sending icw1 to pic1
    outportb( PIC1_REG_COMMAND,0x11 );  

    // mapping irq 0..7 to iterrupt no master_base
    outportb( PIC1_REG_DATA,MASTER_PIC_BASE );

    // sending icw3
    outportb( PIC1_REG_DATA,0x4 ); // 0x4 = 0100 secound bit ( IR line 2)

    // sending icw4
    outportb( PIC1_REG_DATA,0x1 ); // controller automically perform EOI ( end of interrupt)

    // All done, null out data register
    outportb( PIC1_REG_DATA,0x00 );
    
    
    // sending icw1 to pic2
    outportb( PIC2_REG_COMMAND,0x11 ); 

    // mapping irq 8..15 to interrupt no slave_base
    outportb( PIC2_REG_DATA,SLAVE_PIC_BASE );

    // sending icw3
    outportb( PIC2_REG_DATA,0x02 ); // 0x2 = 010 ir lien 2

    // sending icw4
    outportb( PIC2_REG_DATA,0x01 );
    
    // All done, null out data register
    outportb( PIC2_REG_DATA,0x00 );
    
    uint8_t flags = IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_BIT32;

    // seting up 
    idt_set_ir( MASTER_PIC_BASE + 0, flags, 0x08 , (uint32_t)irq0 );
    idt_set_ir( MASTER_PIC_BASE + 1, flags, 0x08 , (uint32_t)irq1 );
    idt_set_ir( MASTER_PIC_BASE + 2, flags, 0x08 , (uint32_t)irq2 );
    idt_set_ir( MASTER_PIC_BASE + 3, flags, 0x08 , (uint32_t)irq3 );
    idt_set_ir( MASTER_PIC_BASE + 4, flags, 0x08 , (uint32_t)irq4 );
    idt_set_ir( MASTER_PIC_BASE + 5, flags, 0x08 , (uint32_t)irq5 );
    idt_set_ir( MASTER_PIC_BASE + 6, flags, 0x08 , (uint32_t)irq6 );
    idt_set_ir( MASTER_PIC_BASE + 7, flags, 0x08 , (uint32_t)irq7 );
    idt_set_ir( SLAVE_PIC_BASE + 0,  flags, 0x08 , (uint32_t)irq8 );
    idt_set_ir( SLAVE_PIC_BASE + 1,  flags, 0x08 , (uint32_t)irq9 );
    idt_set_ir( SLAVE_PIC_BASE + 2,  flags, 0x08 , (uint32_t)irq10 );
    idt_set_ir( SLAVE_PIC_BASE + 3,  flags, 0x08 , (uint32_t)irq11 );
    idt_set_ir( SLAVE_PIC_BASE + 4,  flags, 0x08 , (uint32_t)irq12 );
    idt_set_ir( SLAVE_PIC_BASE + 5,  flags, 0x08 , (uint32_t)irq13 );
    idt_set_ir( SLAVE_PIC_BASE + 6,  flags, 0x08 , (uint32_t)irq14 );
    idt_set_ir( SLAVE_PIC_BASE + 7,  flags, 0x08 , (uint32_t)irq15 );
    
    register_isr(33,keyboard_handler);
    
}


