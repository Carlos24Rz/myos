#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stddef.h>

/*
 * Serial Ports
 *
 * These are legacy communication ports to external devices.
 *
 * Serial Ports are mapped to I/O ports
 */

#define SERIAL_COM1_PORT 0x3F8

/**
 * @brief Initalize a serial port
 *
 * @details
 * This function configures and initializes a serial port
 * to be ready for transmission
 * 
 * @param port The serial port to configure
 */
void serial_initialize_port(uint16_t port);

/**
 * @brief Write a ASCII character into a serial port
 *
 * @param port The serial port to write to
 * @param c    The ASCII character to write
 */
void serial_put(uint16_t port, char c);

/**
 * @brief Write a string into a serial port
 *
 * @param port The serial port to write to
 * @param str  Null-terminated string
 * @param strlen Maximum number of characters to write
 */
void serial_write(uint16_t port, unsigned char *str, size_t strlen);

#endif /* SERIAL_H */
