#include "framebuffer.h"

#define GREETINGS_LEN 12

/*
 * The C kernel entrypoint
 */
void kmain(void)
{
  unsigned char greetings[GREETINGS_LEN + 1] = "Hello World!";
  fb_write(greetings, GREETINGS_LEN);
}
