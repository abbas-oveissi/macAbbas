#include "system.h"



uint32_t kmalloc(uint32_t sz)
{
  uint32_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}

uint32_t kmalloc(uint32_t sz, int align)
{
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  uint32_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}


uint32_t kmalloc(uint32_t sz, int align, uint32_t *phys)
{
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys)
  {
    *phys = placement_address;
  }
  uint32_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}