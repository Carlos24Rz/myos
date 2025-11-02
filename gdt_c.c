#include <stdint.h>
#include <stddef.h>
#include "gdt.h"

/*
 * @copy_docs initialize_gdt_entry
 */
void initialize_gdt_entry(gdt_entry *entry, uint32_t base_address, uint32_t limit, uint8_t access_byte)
{
  if (entry == NULL) return;

  /* Limit should only be 20 bits */
  limit &= 0xFFFFF;

  /* Segment flags (occupy the highest nibble of flags_high_limit_gdt_entry)
   * 3 |  2 | 1 |    0
   * G | DB | L | Reserved
   *
   * G (Granularity) -> 0 for 1 byte blocks, 1 for 4 kb blocks
   * DB (Segment Size) -> 0 for 16 bit segment, 1 for 32 bits
   * L (Long mode) -> 1 for 64-bit segment (DB must be zero), 0 for 32 bits
   * Reserved is for future use
   */

  /* Set entry flags:
   * - 4 kb blocks granularity
   * - 32 bit segment
   */
  entry->flags_high_limit_gdt_entry = 0xC << 4;

  entry->access_byte_gdt_entry = access_byte;

  entry->low_base_gdt_entry = base_address & 0xFFFF;
  entry->middle_base_gdt_entry = (base_address >> 16) & 0xFF;
  entry->high_base_gdt_entry = base_address >> 24;

  entry->low_limit_gdt_entry = limit & 0xFFFF;
  entry->flags_high_limit_gdt_entry |= (limit >> 16) & 0xF;
}
