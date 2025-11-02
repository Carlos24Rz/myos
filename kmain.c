#include <stdint.h>

#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"

#define STRLEN(str) (sizeof(str)/sizeof(str[0]) - 1)

gdt_entry gdt_table[3] = {0};
gdt gdt_descriptor;

/*
 * The C kernel entrypoint
 */
void kmain(void)
{
  unsigned char greetings[] = "Hello World! My name is Carlos\nNice to meet you!";

  /* Initialize Kernel Segments */
  initialize_gdt_entry(&gdt_table[1], 0, 0xFFFFF, KERNEL_CODE_SEGMENT);
  initialize_gdt_entry(&gdt_table[2], 0, 0xFFFFF, KERNEL_DATA_SEGMENT);

  gdt_descriptor.size_gdt = sizeof(gdt_table) - 1;
  gdt_descriptor.base_address_gdt = (uintptr_t)&gdt_table[0];

  lgdt(&gdt_descriptor);

  reload_segment_registers();

  fb_write(greetings, STRLEN(greetings));

  serial_initialize_port(SERIAL_COM1_PORT);

  serial_write(SERIAL_COM1_PORT, greetings, STRLEN(greetings));
}
