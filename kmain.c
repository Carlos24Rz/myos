#include "framebuffer.h"
#include "serial.h"

#define STRLEN(str) (sizeof(str)/sizeof(str[0]) - 1)

/*
 * The C kernel entrypoint
 */
void kmain(void)
{
  unsigned char greetings[] = "Hello World! My name is Carlos\nNice to meet you!";

  fb_write(greetings, STRLEN(greetings));

  serial_initialize_port(SERIAL_COM1_PORT);

  serial_write(SERIAL_COM1_PORT, greetings, STRLEN(greetings));

}
