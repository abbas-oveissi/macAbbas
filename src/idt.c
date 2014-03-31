#include "system.h"
#include <stdint.h>
#include "idt.h"




extern void register_isr( unsigned char n, isr_t handler );
extern void isr_none(registers_t);

//================================
// structs
//================================
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
//================================

isr_t interrupt_handlers[MAX_INTERRUPTS];


extern void idt_install(uint32_t);


void idt_init()
{
    idtp.limit = sizeof(struct idt_entry) * MAX_INTERRUPTS -1;
    idtp.base  = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry)*MAX_INTERRUPTS);
    
    
    uint8_t flags = IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_BIT32;

  // isr
  idt_set_ir( 0, flags, 0x08 , (uint32_t)isr0);
  idt_set_ir( 1, flags, 0x08 , (uint32_t)isr1);
  idt_set_ir( 2, flags, 0x08 , (uint32_t)isr2);
  idt_set_ir( 3, flags, 0x08 , (uint32_t)isr3);
  idt_set_ir( 4, flags, 0x08 , (uint32_t)isr4);
  idt_set_ir( 5, flags, 0x08 , (uint32_t)isr5);
  idt_set_ir( 6, flags, 0x08 , (uint32_t)isr6);
  idt_set_ir( 7, flags, 0x08 , (uint32_t)isr7);
  idt_set_ir( 8, flags, 0x08 , (uint32_t)isr8);
  idt_set_ir( 9, flags, 0x08 , (uint32_t)isr9);
  idt_set_ir( 10, flags, 0x08 , (uint32_t)isr10);
  idt_set_ir( 11, flags, 0x08 , (uint32_t)isr11);
  idt_set_ir( 12, flags, 0x08 , (uint32_t)isr12);
  idt_set_ir( 13, flags, 0x08 , (uint32_t)isr13);
  idt_set_ir( 14, flags, 0x08 , (uint32_t)isr14);
  idt_set_ir( 15, flags, 0x08 , (uint32_t)isr15);
  idt_set_ir( 16, flags, 0x08 , (uint32_t)isr16);
  idt_set_ir( 17, flags, 0x08 , (uint32_t)isr17);
  idt_set_ir( 18, flags, 0x08 , (uint32_t)isr18);
  idt_set_ir( 19, flags, 0x08 , (uint32_t)isr19);
  idt_set_ir( 20, flags, 0x08 , (uint32_t)isr20);
  idt_set_ir( 21, flags, 0x08 , (uint32_t)isr21);
  idt_set_ir( 22, flags, 0x08 , (uint32_t)isr22);
  idt_set_ir( 23, flags, 0x08 , (uint32_t)isr23);
  idt_set_ir( 24, flags, 0x08 , (uint32_t)isr24);
  idt_set_ir( 25, flags, 0x08 , (uint32_t)isr25);
  idt_set_ir( 26, flags, 0x08 , (uint32_t)isr26);
  idt_set_ir( 27, flags, 0x08 , (uint32_t)isr27);
  idt_set_ir( 28, flags, 0x08 , (uint32_t)isr28);
  idt_set_ir( 29, flags, 0x08 , (uint32_t)isr29);
  idt_set_ir( 30, flags, 0x08 , (uint32_t)isr30);
  idt_set_ir( 31, flags, 0x08 , (uint32_t)isr31);

  // system calls interrupt vector
   // idt_set_ir( 80, flags, 0x08 , (uint32_t)isr80 );

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







