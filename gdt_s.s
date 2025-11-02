/*
 * @brief Load Global Descriptor Table into CPU
 *
 * @param [esp + 4] Pointer to GDT
 */
.global lgdt
lgdt:
  movl 4(%esp), %eax
  lgdt (%eax)
  ret

/*
 * @brief Reload segment registers
 * 
 * @details This should be executed right after lgdt has been executed to update the segment registers
 */
.global reload_segment_registers
reload_segment_registers:
  ljmp $0x08, $flush_cs   /* specify cs when jumping to flush_cs */

  flush_cs:               /* now we've changed cs to 0x08 */
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ret
