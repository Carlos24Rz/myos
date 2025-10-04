#include <stdint.h>
#include <stddef.h>
#include "io.h"

/* Framebuffer Cursor I/O ports */
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5

/* High/Low data byte commands */
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND  15

/* Framebuffer provides a 80 x 25 grid */
#define VGA_MAX_COLUMNS 80
#define VGA_MAX_ROWS    25

/* Supported vga colors for both foreground and background */
typedef enum VGA_COLORS {
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
} VGA_COLORS;

typedef struct vga_color_t
{
  VGA_COLORS fg_vga_color;
  VGA_COLORS bg_vga_color;
} vga_color_t;

/* Global variables  */

/* Framebuffer memory mapped starting address */
static uint16_t *vga_framebuffer = (uint16_t *)0x000B8000;

/* VGA current column and row index */
static unsigned int vga_column_idx = 0;
static unsigned int vga_row_idx    = 0;

/*************************
 * Function declarations *
 *************************/
void fb_update_cursor(uint8_t col_idx, uint8_t row_idx);
void fb_put_at(unsigned char c, vga_color_t color, uint16_t pos);
void fb_put(unsigned char c, vga_color_t color);
void fb_scroll();

/*****************************
 * End function declarations *
 *****************************/

/*
 * @brief Update framebuffer cursor position
 *
 * @param col_idx The column index to update the cursor to
 * @param row_idx The row index to update the cursor to
 *
 */
void fb_update_cursor(uint8_t col_idx, uint8_t row_idx)
{
  uint16_t position;

  if (col_idx >= VGA_MAX_COLUMNS || row_idx >= VGA_MAX_ROWS) return;

  position = col_idx + row_idx * VGA_MAX_COLUMNS;

  /* The cursor position is updated by sending the position
   * to the framebuffer i/o port */

  /* Send high byte of the position */
  outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
  outb(FB_DATA_PORT, (position >> 8) & 0xFF);

  /* Send low byte of the position */
  outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
  outb(FB_DATA_PORT, position & 0xFF);
}

/*
 * @brief Write a ASCII character into the framebuffer position
 *
 * @details
 * This routine does not update the framebuffer position
 *
 * @param c The ASCII character to write
 * @param color The color settings for the character
 * @param pos The framebuffer position to write the character
 */
void fb_put_at(unsigned char c, vga_color_t color, uint16_t pos)
{
  if (pos >= VGA_MAX_COLUMNS * VGA_MAX_ROWS) return;

  /* Each framebuffer cell is 16 bits:
   * - Low byte: ASCII character
   * - High byte: foreground and background colors
   */
  vga_framebuffer[pos] =
    c | (color.fg_vga_color | (color.bg_vga_color << 4)) << 8;
}

/*
 * @brief Write a ASCII character into the framebuffer
 *
 * @details
 * This routine updates the framebuffer position.
 * If the framebuffer is full, the screen will be scrolled
 *
 * @param c The ASCII character to write
 * @param color The color settings for the character
 */
void fb_put(unsigned char c, vga_color_t color)
{
  fb_put_at(c, color, vga_column_idx + vga_row_idx * VGA_MAX_COLUMNS);

  /* Move to the next row at the end of the line */
  if (++vga_column_idx == VGA_MAX_COLUMNS)
  {
     vga_column_idx = 0;

     /* Scroll screen if where at the end of the framebuffer */
     if (++vga_row_idx == VGA_MAX_ROWS)
     {
       fb_scroll();
       vga_row_idx--;
     }
  }

  /* Update cursor */
  fb_update_cursor(vga_column_idx, vga_row_idx);
}

/*
 * @brief Scrolls the framebuffer screen up by one row
 */
void fb_scroll()
{
  vga_color_t black_color = {VGA_COLOR_BLACK, VGA_COLOR_BLACK};
  uint16_t *current_fb_ptr = vga_framebuffer;

  /* Scroll all but last row */
  while (current_fb_ptr != vga_framebuffer + (VGA_MAX_ROWS - 1) * VGA_MAX_COLUMNS)
  {
    /* Copy next column row cell into current column */
    *current_fb_ptr = *(current_fb_ptr + VGA_MAX_COLUMNS);
    current_fb_ptr++;
  }

  /* Clear last row */
  while (current_fb_ptr != vga_framebuffer + VGA_MAX_ROWS * VGA_MAX_COLUMNS)
  {
    fb_put_at(' ', black_color, current_fb_ptr-vga_framebuffer);
    current_fb_ptr++;
  }
}

/*
 * @copydoc fb_write
 */
void fb_write(unsigned char *str, size_t strlen)
{
  size_t i;
  vga_color_t screen_color = {VGA_COLOR_WHITE, VGA_COLOR_BLACK};

  for (i = 0; i < strlen && str[i] != '\0'; i++)
  {
     fb_put(str[i], screen_color);
  }
}
