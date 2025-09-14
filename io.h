#ifndef KERNEL_IO_H
#define KERNEL_IO_H

#include <stdint.h>

/**
 * @brief outb - Send a byte to a I/O port
 * 
 * @param port_address the I/O port
 * @param data the data byte
 */
void outb(uint16_t port_address, uint8_t data);

#endif /* KERNEL_IO_H */
