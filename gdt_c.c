#include <stdint.h>
#include <stddef.h>
#include "gdt.h"

void initialize_gdt_entry(gdt_entry *entry, uint32_t base_address, uint32_t limit, uint8_t access_byte)
{
  if (entry == NULL) return;

  /* Limit should only be 20 bits */
  limit &= 0xFFFFF;

  /* Set entry flags:
   * - 1 byte granularity
   * - 32 bit segment
   */
  entry->flags_high_limit_gdt_entry = 0x2 << 4;

  entry->access_byte_gdt_entry = access_byte;

  entry->low_base_gdt_entry = base_address & 0xFFFF;
  entry->middle_base_gdt_entry = (base_address >> 16) & 0xFF;
  entry->high_base_gdt_entry = base_address >> 24;

  entry->low_limit_gdt_entry = limit & 0xFFFF;
  entry->flags_high_limit_gdt_entry |= (limit >> 16) & 0xF;
}
