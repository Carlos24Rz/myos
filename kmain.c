#include <stdint.h>

#define VGA_MAX_COLUMNS 80
#define VGA_MAX_ROWS    25

uint16_t *vga_framebuffer = (uint16_t *)0x000B8000;

unsigned int vga_column_idx = 0;
unsigned int vga_row_idx    = 0;

enum VGA_COLORS {
  VGA_COLOR_BLACK = 0,
  VGA_COLOR_BLUE,
  VGA_COLOR_GREEN,
  VGA_COLOR_CYAN,
  VGA_COLOR_RED,
  VGA_COLOR_MAGENTA,
  VGA_COLOR_BROWN,
  VGA_COLOR_LIGHT_GREY,
  VGA_COLOR_DARK_GREY,
  VGA_COLOR_LIGHT_BLUE,
  VGA_COLOR_LIGHT_GREEN,
  VGA_COLOR_LIGHT_CYAN,
  VGA_COLOR_LIGHT_RED,
  VGA_COLOR_LIGHT_MAGENTA,
  VGA_COLOR_LIGHT_BROWN,
  VGA_COLOR_WHITE,
};

void terminal_put(char c)
{
  vga_framebuffer[vga_column_idx + vga_row_idx * VGA_MAX_COLUMNS] =
    c | (VGA_COLOR_WHITE | (VGA_COLOR_BLACK << 4)) << 8;

  if (++vga_column_idx == VGA_MAX_COLUMNS)
  {
    vga_column_idx = 0;

    if (++vga_row_idx == VGA_MAX_ROWS)
    {
      vga_row_idx = 0;
    }
  }
}

void terminal_puts(char *str)
{
  while (*str != '\0')
  {
    terminal_put(*str);
    str++;
  }
}


/*
 * The C kernel entrypoint
 */
void kmain(void)
{
  terminal_puts("Hello World!");

}
