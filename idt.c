#include "system.h"

#include <stdint.h>


#define IDT_DESC_BIT16    0x06  //00000110
#define IDT_DESC_BIT32    0x0E  //00001110

#define IDT_DESC_RING0    0x0   //00000000
#define IDT_DESC_RING1    0x40  //01000000
#define IDT_DESC_RING2    0x20  //00100000
#define IDT_DESC_RING3    0x60  //01100000

#define IDT_DESC_PRESENT  0x80  //10000000
extern void register_isr( uint8_t n, isr_t handler );

//================================
// exceptions
//================================
extern void isr0 ();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void isr_none(registers_t);


//================================
// irqs
//================================
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

//================================
// other interrupt
//================================

//! system call handler
extern void isr80();

struct idt_entry
{
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
} __attribute__((packed));

struct idt_ptr
{
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));


static struct idt_entry idt[MAX_INTERRUPTS];
static struct idt_ptr idtp;
isr_t interrupt_handlers[MAX_INTERRUPTS];


extern void idt_install(uint32_t);


void idt_init(uint16_t _codeSel)
{
    idtp.limit = sizeof(struct idt_entry) * MAX_INTERRUPTS -1;
    idtp.base  = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry)*MAX_INTERRUPTS);
    
    
    uint8_t flags = IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_BIT32;

  // isr
  idt_set_ir( 0, flags, _codeSel , (uint32_t)isr0);
  idt_set_ir( 1, flags, _codeSel , (uint32_t)isr1);
  idt_set_ir( 2, flags, _codeSel , (uint32_t)isr2);
  idt_set_ir( 3, flags, _codeSel , (uint32_t)isr3);
  idt_set_ir( 4, flags, _codeSel , (uint32_t)isr4);
  idt_set_ir( 5, flags, _codeSel , (uint32_t)isr5);
  idt_set_ir( 6, flags, _codeSel , (uint32_t)isr6);
  idt_set_ir( 7, flags, _codeSel , (uint32_t)isr7);
  idt_set_ir( 8, flags, _codeSel , (uint32_t)isr8);
  idt_set_ir( 9, flags, _codeSel , (uint32_t)isr9);
  idt_set_ir( 10, flags, _codeSel , (uint32_t)isr10);
  idt_set_ir( 11, flags, _codeSel , (uint32_t)isr11);
  idt_set_ir( 12, flags, _codeSel , (uint32_t)isr12);
  idt_set_ir( 13, flags, _codeSel , (uint32_t)isr13);
  idt_set_ir( 14, flags, _codeSel , (uint32_t)isr14);
  idt_set_ir( 15, flags, _codeSel , (uint32_t)isr15);
  idt_set_ir( 16, flags, _codeSel , (uint32_t)isr16);
  idt_set_ir( 17, flags, _codeSel , (uint32_t)isr17);
  idt_set_ir( 18, flags, _codeSel , (uint32_t)isr18);
  idt_set_ir( 19, flags, _codeSel , (uint32_t)isr19);
  idt_set_ir( 20, flags, _codeSel , (uint32_t)isr20);
  idt_set_ir( 21, flags, _codeSel , (uint32_t)isr21);
  idt_set_ir( 22, flags, _codeSel , (uint32_t)isr22);
  idt_set_ir( 23, flags, _codeSel , (uint32_t)isr23);
  idt_set_ir( 24, flags, _codeSel , (uint32_t)isr24);
  idt_set_ir( 25, flags, _codeSel , (uint32_t)isr25);
  idt_set_ir( 26, flags, _codeSel , (uint32_t)isr26);
  idt_set_ir( 27, flags, _codeSel , (uint32_t)isr27);
  idt_set_ir( 28, flags, _codeSel , (uint32_t)isr28);
  idt_set_ir( 29, flags, _codeSel , (uint32_t)isr29);
  idt_set_ir( 30, flags, _codeSel , (uint32_t)isr30);
  idt_set_ir( 31, flags, _codeSel , (uint32_t)isr31);

  // system calls interrupt vector
    idt_set_ir( 80, flags, _codeSel , (uint32_t)isr80 );

    idt_install( (uint32_t)&idtp );
    
    memset(&interrupt_handlers,0, sizeof(isr_t)*MAX_INTERRUPTS);

}

// idt_set_ir
void idt_set_ir( uint8_t _num, uint8_t _flags, uint16_t _sel, uint32_t _handler)
{
    idt[_num].offset_1 = _handler & 0xFFFF;
    idt[_num].offset_2 = (_handler >> 16) & 0xFFFF;

    idt[_num].selector     = _sel;
    idt[_num].zero = 0;
    idt[_num].type_attr   = _flags;

}







