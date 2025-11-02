#ifndef GDT_H
#define GDT_H

#include <stdint.h>

#define PRESENT_ACCESS_GDT_ENTRY              (0x80) /* Sets segment as a valid entry */
#define LEVEL_0_DPL_ACCESS_GDT_ENTRY          (0x00) /* Ring 0 privilege (highest) */
#define LEVEL_1_DPL_ACCESS_GDT_ENTRY          (0x20) /* Ring 1 privilege */
#define LEVEL_2_DPL_ACCESS_GDT_ENTRY          (0x40) /* Ring 2 privilege */
#define LEVEL_3_DPL_ACCESS_GDT_ENTRY          (0x60) /* Ring 3 privilege (lowest) */
#define SEGMENT_TYPE_ACCESS_GDT_ENTRY         (0x10) /* Segment is code/data (Opposite is system segment type) */
#define EXECUTABLE_ACCESS_GDT_ENTRY           (0x08) /* Segment is for code */
#define DIRECTION_CONFORMING_ACCESS_GDT_ENTRY (0x04) /* Segment grows up */
#define READ_WRITE_ACCESS_GDT_ENTRY           (0x02) /* Segment is readable (for data segment), or writable (for code) */ 
#define ACCESSED_ACCESS_GDT_ENTRY              (0x01) /* Segment has been accessed */

#define KERNEL_CODE_SEGMENT \
  ( PRESENT_ACCESS_GDT_ENTRY | \
    LEVEL_0_DPL_ACCESS_GDT_ENTRY | \
    SEGMENT_TYPE_ACCESS_GDT_ENTRY | \
    EXECUTABLE_ACCESS_GDT_ENTRY | \
    READ_WRITE_ACCESS_GDT_ENTRY | \
    ACCESSED_ACCESS_GDT_ENTRY )

#define KERNEL_DATA_SEGMENT \
  ( PRESENT_ACCESS_GDT_ENTRY | \
    LEVEL_0_DPL_ACCESS_GDT_ENTRY | \
    SEGMENT_TYPE_ACCESS_GDT_ENTRY | \
    READ_WRITE_ACCESS_GDT_ENTRY | \
    ACCESSED_ACCESS_GDT_ENTRY )

/**
 * @brief GDT segment descriptor entry
 */
typedef struct gdt_entry
{
  uint16_t low_limit_gdt_entry;
  uint16_t low_base_gdt_entry;
  uint8_t middle_base_gdt_entry;
  uint8_t access_byte_gdt_entry;
  uint8_t flags_high_limit_gdt_entry;
  uint8_t high_base_gdt_entry;
} __attribute__((packed)) gdt_entry;

/**
 * @brief Global Descriptor Table
 */
typedef struct gdt
{
  uint32_t address_gdt;
  uintptr_t size_gdt;
} __attribute__((packed)) gdt;

void initialize_gdt_entry (gdt_entry *entry, uint32_t base_address, uint32_t limit, uint8_t access_byte);

/*
 * @brief Load Global Descriptor Table into CPU
 *
 * @param new_table Pointer to GDT
 */
void lgdt(gdt *new_table);

/*
 * @brief Reload segment registers
 * 
 * @details This should be executed right after lgdt has been executed to update the segment registers
 */
void reload_segment_registers(void);

#endif
