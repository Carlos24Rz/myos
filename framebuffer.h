#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stddef.h>

/**
 * @brief Writes a string to the framebuffer
 *
 * @param str Pointer to the string to write.
 * @param strlen Number of characters to write from the string.
 *
 * @note If strlen is longer than the actual string length, the function
 *       stops at the first null terminator ('\0').
 */
void fb_write(unsigned char *str, size_t strlen);

#endif /* FRAMMEBUFFER_H */
