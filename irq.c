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
    terminal_writestring("isr_handler\n");
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        terminal_writestring("Recieved interrupt: 1");
    }
}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{   
    terminal_writestring("irq_handler\n");
    // sending EOI (end of interrup) command to the pic
    pic_interrupt_done(regs.int_no);
    
    // call cusomized handler if found
    if (interrupt_handlers[regs.int_no] != 0)
    {
  terminal_writestring("asas");
        
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
    else
    {
        terminal_writestring("Recieved IRQ: 1");
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
    
}

// pic_interrupt_done
void pic_interrupt_done(u32_t int_no)
{
    if ( int_no >= SLAVE_PIC_BASE )
    {
        // Send reset signal to slave.
        outportb( PIC2_REG_COMMAND, 0x20 );
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outportb( PIC1_REG_COMMAND, 0x20 );
}



/* These are own ISRs that point to our special IRQ handler
*  instead of the regular 'fault_handler' function 
extern void irq1();

/* This array is actually an array of function pointers. We use
*  this to handle custom IRQ handlers for a given IRQ 
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ 
void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ 
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}

/* Normally, IRQs 0 to 7 are mapped to entries 8 to 15. This
*  is a problem in protected mode, because IDT entry 8 is a
*  Double Fault! Without remapping, every time IRQ0 fires,
*  you get a Double Fault Exception, which is NOT actually
*  what's happening. We send commands to the Programmable
*  Interrupt Controller (PICs - also called the 8259's) in
*  order to make IRQ0 to 15 be remapped to IDT entries 32 to
*  47 
void irq_remap(void)
{
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x0);
    outportb(0xA1, 0x0);
}

/* We first remap the interrupt controllers, and then we install
*  the appropriate ISRs to the correct entries in the IDT. This
*  is just like installing the exception handlers
void irq_install()
{
    irq_remap();
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
}

/* Each of the IRQ ISRs point to this function, rather than
*  the 'fault_handler' in 'isrs.c'. The IRQ Controllers need
*  to be told when you are done servicing them, so you need
*  to send them an "End of Interrupt" command (0x20). There
*  are two 8259 chips: The first exists at 0x20, the second
*  exists at 0xA0. If the second controller (an IRQ from 8 to
*  15) gets an interrupt, you need to acknowledge the
*  interrupt at BOTH controllers, otherwise, you only send
*  an EOI command to the first controller. If you don't send
*  an EOI, you won't raise any more IRQs 
void irq_handler(struct regs *r)
{

    /* This is a blank function pointer 
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it 
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller 
    if (r->int_no >= 40)
    {
        outportb(0xA0, 0x20);
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too 
    outportb(0x20, 0x20);
}*/