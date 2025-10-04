/**
 * @brief outb - Send a byte to a I/O port
 * 
 * @param [esp + 4] the I/O port
 * @param [esp + 8] the data byte
 */
.global outb
outb:
  movw 4(%esp), %dx
  movb 8(%esp), %al
  outb %al,    %dx
  ret

/**
 * @brief inb - Reads a byte from a I/O port
 * 
 * @param [esp + 4] the I/O port
 */
.global inb
inb:
  movw 4(%esp), %dx
  inb %dx, %al
  ret
