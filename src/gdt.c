#include "system.h"
#include <stdint.h>
#include <stdbool.h>


//================================
// structs
//================================
struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct gdt_entry gdt[GDT_DESCRIPTORS_NO];
struct gdt_ptr gp;
//================================



//! gdt_inistall, defined in gdt.s
extern void gdt_install(uint32_t);

//! Setup a descriptor in the GDT
static void gdt_set_gate(uint32_t  desc_index, uint32_t  base_address, uint32_t  limit, uint8_t  access, uint8_t grand);

// gdtSetDescriptor
void gdt_set_gate(uint32_t  desc_index, uint32_t  base_address, uint32_t  limit, uint8_t  access, uint8_t grand)
{
    if ( desc_index > GDT_DESCRIPTORS_NO )
        return;
    memset ( (void*)&gdt[desc_index], 0, sizeof (struct gdt_entry) );
    uint32_t w = base_address;
    gdt[desc_index].base_low = (uint16_t)(base_address & 0xffff);
    gdt[desc_index].base_middle    = (uint8_t)(w >> 16);
    gdt[desc_index].base_high = (uint8_t)(w >> 24);
    gdt[desc_index].limit_low  = (uint16_t)(limit & 0xffff);
    gdt[desc_index].access = access;
    w = limit;
    gdt[desc_index].granularity = (uint8_t)((w >> 16) & 0x0f);
    gdt[desc_index].granularity |= grand & 0xf0;
}


void gdt_init()
{
    gp.limit = (sizeof(struct gdt_entry) * GDT_DESCRIPTORS_NO) - 1;
    gp.base = (uint32_t)&gdt;
    gdt_set_gate(0, 0, 0, 0, 0);
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    gdt_install((uint32_t)&gp);
}

