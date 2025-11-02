#ifndef GDT_H
#define GDT_H

#include <stdint.h>

/*
 * Global Descriptor Table (GDT)
 *
 * In x86 processors, memory addressing originally used a mechanism called segmentation.
 *
 * Segmentation allowed early 16-bit CPUs (like the Intel 8086) to extend usable memory
 * beyond the 64 KB limit of a 16-bit address space by combining a 16-bit segment base
 * with a 16-bit offset, giving access to up to 1 MB of memory.
 *
 * In 32-bit protected mode, segmentation evolved to use segment descriptors stored in
 * the Global Descriptor Table (GDT). Each descriptor defines:
 *   - A base address (starting point of the segment)
 *   - A limit (size of the segment)
 *   - Access rights and privilege level (ring 0–3)
 *
 * Although modern 32-bit and 64-bit systems typically use flat segmentation
 * (all bases set to 0 and limits to 4 GB), the GDT remains a required structure
 * for defining privilege levels and task state segments.
 */

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
 * @brief GDT Descriptor
 *
 * @details This descriptor is used by the lgdt instruction
 *          to locate where is the GDT
 */
typedef struct gdt
{
  uint16_t size_gdt;
  uintptr_t base_address_gdt;
} __attribute__((packed)) gdt;


/* Segment descriptor access
 *    7 | 6 | 5 | 4 | 3 | 2  | 1  | 0 
 *    P |  DPL  | S | E | DC | RW | A
 *
 * P (Present bit) -> 1 to enable segment, 0 to disable it
 * DPL (Descriptor privilege level) -> Ring privilege (0-3)
 * S (Segment type) -> 0 for task segment, 1 for code/data segment
 * E (Executable) -> 0 for data segment, 1 for code (executable) segment
 * D (Direction bit/Conforming):
 *  - If segment is data: 0 if segment grows up, 1 if it grows downwards
 *  - If segment is code: 0 segment can be accessed from equal or higher levels (unsafe),
                          1 if segment can be accessed from equal or lower levels
 * RW (Read/Write):
 *  - If segment is data: 1 if segment can be written, 0 otherwise.
 *  - If segment is code: 1 if segment can be read, 0 otherwise
 * A (Accessed): Set be CPU when segment is accessed
 */
#define PRESENT_ACCESS_GDT_ENTRY              (0x80) /* Sets segment as a valid entry */
#define LEVEL_0_DPL_ACCESS_GDT_ENTRY          (0x00) /* Ring 0 privilege (highest) */
#define LEVEL_1_DPL_ACCESS_GDT_ENTRY          (0x20) /* Ring 1 privilege */
#define LEVEL_2_DPL_ACCESS_GDT_ENTRY          (0x40) /* Ring 2 privilege */
#define LEVEL_3_DPL_ACCESS_GDT_ENTRY          (0x60) /* Ring 3 privilege (lowest) */
#define SEGMENT_TYPE_ACCESS_GDT_ENTRY         (0x10) /* Segment is code/data (Opposite is system segment type) */
#define EXECUTABLE_ACCESS_GDT_ENTRY           (0x08) /* Segment is for code */
#define DIRECTION_CONFORMING_ACCESS_GDT_ENTRY (0x04) /* Segment grows up */
#define READ_WRITE_ACCESS_GDT_ENTRY           (0x02) /* Segment is readable (for data segment), or writable (for code) */ 

/*
 * @brief Shortcut for Kernel code segment
 */
#define KERNEL_CODE_SEGMENT \
  ( PRESENT_ACCESS_GDT_ENTRY | \
    LEVEL_0_DPL_ACCESS_GDT_ENTRY | \
    SEGMENT_TYPE_ACCESS_GDT_ENTRY | \
    EXECUTABLE_ACCESS_GDT_ENTRY | \
    READ_WRITE_ACCESS_GDT_ENTRY )

/*
 * @brief Shortcut for Kernel data segment
 */
#define KERNEL_DATA_SEGMENT \
  ( PRESENT_ACCESS_GDT_ENTRY | \
    LEVEL_0_DPL_ACCESS_GDT_ENTRY | \
    SEGMENT_TYPE_ACCESS_GDT_ENTRY | \
    READ_WRITE_ACCESS_GDT_ENTRY )

/*
 * @brief Initialize a segment entry
 *
 * @param entry        The segment entry to initialize
 * @param base_address The segment base address
 * @param limit        20-bit segment address limit
 * @param access_byte  Segment access configuration
 */
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
