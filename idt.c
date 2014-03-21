typedef unsigned long addr_t;

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


struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_install(addr_t);

/* Installs the IDT 
void idt_install()
{
    /* Sets the special IDT pointer up, just like in 'gdt.c' 
    idtp.limit = (sizeof (struct idt_entry) * 256) - 1;
    idtp.base = &idt;

    /* Clear out the entire IDT, initializing it to zeros 
    memset(&idt, 0, sizeof(struct idt_entry) * 256);

    /* Add any new ISRs to the IDT here using idt_set_gate 

    /* Points the processor's internal register to the new IDT 
    idt_load(&idtp);
}*/

void idt_init(uint16_t _codeSel)
{
    idt_ptr.limit = sizeof(idt_descriptor_t) * MAX_INTERRUPTS -1;
    idt_ptr.base  = (u32_t)&idt_entries;

    memset(&idt_entries, 0, sizeof(idt_descriptor_t)*MAX_INTERRUPTS);
    
    
    u8_t flags = IDT_DESC_PRESENT | IDT_DESC_RING0 | IDT_DESC_BIT32;

  // isr
  idt_set_ir( 0, flags, _codeSel , (addr_t)isr0);
  idt_set_ir( 1, flags, _codeSel , (addr_t)isr1);
  idt_set_ir( 2, flags, _codeSel , (addr_t)isr2);
  idt_set_ir( 3, flags, _codeSel , (addr_t)isr3);
  idt_set_ir( 4, flags, _codeSel , (addr_t)isr4);
  idt_set_ir( 5, flags, _codeSel , (addr_t)isr5);
  idt_set_ir( 6, flags, _codeSel , (addr_t)isr6);
  idt_set_ir( 7, flags, _codeSel , (addr_t)isr7);
  idt_set_ir( 8, flags, _codeSel , (addr_t)isr8);
  idt_set_ir( 9, flags, _codeSel , (addr_t)isr9);
  idt_set_ir( 10, flags, _codeSel , (addr_t)isr10);
  idt_set_ir( 11, flags, _codeSel , (addr_t)isr11);
  idt_set_ir( 12, flags, _codeSel , (addr_t)isr12);
  idt_set_ir( 13, flags, _codeSel , (addr_t)isr13);
  idt_set_ir( 14, flags, _codeSel , (addr_t)isr14);
  idt_set_ir( 15, flags, _codeSel , (addr_t)isr15);
  idt_set_ir( 16, flags, _codeSel , (addr_t)isr16);
  idt_set_ir( 17, flags, _codeSel , (addr_t)isr17);
  idt_set_ir( 18, flags, _codeSel , (addr_t)isr18);
  idt_set_ir( 19, flags, _codeSel , (addr_t)isr19);
  idt_set_ir( 20, flags, _codeSel , (addr_t)isr20);
  idt_set_ir( 21, flags, _codeSel , (addr_t)isr21);
  idt_set_ir( 22, flags, _codeSel , (addr_t)isr22);
  idt_set_ir( 23, flags, _codeSel , (addr_t)isr23);
  idt_set_ir( 24, flags, _codeSel , (addr_t)isr24);
  idt_set_ir( 25, flags, _codeSel , (addr_t)isr25);
  idt_set_ir( 26, flags, _codeSel , (addr_t)isr26);
  idt_set_ir( 27, flags, _codeSel , (addr_t)isr27);
  idt_set_ir( 28, flags, _codeSel , (addr_t)isr28);
  idt_set_ir( 29, flags, _codeSel , (addr_t)isr29);
  idt_set_ir( 30, flags, _codeSel , (addr_t)isr30);
  idt_set_ir( 31, flags, _codeSel , (addr_t)isr31);

  // system calls interrupt vector
    idt_set_ir( SYS_CALL_INTERRUPT, flags, _codeSel , (addr_t)isr80 );

    idt_install( (addr_t)&idt_ptr );
    
    memset(&interrupt_handlers,0, sizeof(isr_t)*MAX_INTERRUPTS);

  return TRUE;
}

// idt_set_ir
void idt_set_ir( u8_t _num, u8_t _flags, u16_t _sel, addr_t _handler)
{
    idt_entries[_num].baseLo = _handler & 0xFFFF;
    idt_entries[_num].baseHi = (_handler >> 16) & 0xFFFF;

    idt_entries[_num].sel     = _sel;
    idt_entries[_num].reserved = 0;
    idt_entries[_num].flags   = _flags;

}


void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].offset_2 = (base & 0xFFFF);
    idt[num].offset_1 = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].zero = 0;
    idt[num].type_attr = flags;
}





