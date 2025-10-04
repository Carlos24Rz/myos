#include <stdbool.h>
#include "io.h"
#include "serial.h"

/*
 * UART Serial Port registers
 *
 * In order to enable communication with serial ports,
 * both the device and host must agree on a configuration
 * setup.
 *
 * In the UART protocol, the configuration is set using a
 * set of registers, based on the offset to the base I/O port
 * address. Most of the basic configuration is done through the
 * line control register.
 */

#define SERIAL_DATA_REGISTER(port) (port)
#define SERIAL_FIFO_CONTROL_REGISTER(port) (port + 2)
#define SERIAL_LINE_CONTROL_REGISTER(port) (port + 3)
#define SERIAL_MODEM_CONTROL_REGISTER(port) (port + 4)
#define SERIAL_LINE_STATUS_REGISTER(port) (port + 5)
#define SERIAL_MODEM_STATUS_REGISTER(port) (port + 6)

/*************************
 * Function declarations *
 *************************/
void serial_configure_baud(uint16_t port, uint16_t clock_divisor);
void serial_configure_line(uint16_t port);
void serial_configure_buffer(uint16_t port);
void serial_configure_modem(uint16_t port);
bool serial_is_transmit_ready(uint16_t port);

/*****************************
 * End function declarations *
 *****************************/

/*
 * @brief Configure configure baud rate
 *
 * @param port The serial port to configure
 * @param clock_divisor The clock divisor ( baud = clock / divisor )
 */
void serial_configure_baud(uint16_t port, uint16_t clock_divisor)
{
  /* Enable the DLAB bit in line control register (highest bit) */
  outb(SERIAL_LINE_CONTROL_REGISTER(port), 0x80);

  /* Set divisor (DLAB enabled)
   * DLL (offset +0) = low byte, DLH (offest +1) = high byte
   */ 
  outb(SERIAL_DATA_REGISTER(port), clock_divisor & 0xFF);
  outb(SERIAL_DATA_REGISTER(port) + 1, clock_divisor >> 8);

  /* Disable DLAB */
  outb(SERIAL_LINE_CONTROL_REGISTER(port), 0x0);
}

/**
 * @brief Configure serial communication line
 *
 * @details
 * This function sets a data length of 8 bits, no parity bits,
 * one control bit and no break control.
 *
 * @param port The serial port to configure
 */
void serial_configure_line(uint16_t port)
{
  outb(SERIAL_LINE_CONTROL_REGISTER(port), 0x03);
}

/**
 * @brief Configure serial FIFO buffers
 *
 * @details
 * This function sets a buffer size of 14 bytes, no DMA,
 * and clears the receive and transmission buffers for a given
 * serial port.
 *
 * @param port The serial port to configure
 */
void serial_configure_buffer(uint16_t port)
{
  outb(SERIAL_FIFO_CONTROL_REGISTER(port), 0xC7);
}

/**
 * @brief Configure serial modem
 *
 * @details
 * This function sets the serial port for ready-to-transmit
 *
 * @param port The serial port to configure
 */
void serial_configure_modem(uint16_t port)
{
  outb(SERIAL_MODEM_CONTROL_REGISTER(port), 0x03);
}

/**
 * @brief Checks if serial port is ready for transmitting data
 *
 * @details
 * The serial port is considered to be ready for transmission if
 * the fifth bit in the line status register is enabled
 *
 * @param port The serial port to check
 */
bool serial_is_transmit_ready(uint16_t port)
{
  uint8_t result = inb(SERIAL_LINE_STATUS_REGISTER(port));

  /* FIFO queue is empty if the fifth bit is one */
  return !!(result & 0x20);
}

/**
 * @copydoc serial_initialize_port
 */
void serial_initialize_port(uint16_t port)
{
  /* Set baud rate to 115200 / 2 = 57600 Hz */
  serial_configure_baud(port, 2);

  serial_configure_line(port);
  serial_configure_buffer(port);

  /* Ready for transmission */
  serial_configure_modem(port);
}

/*
 * @copydoc serial_put
 */
void serial_put(uint16_t port, char c)
{
  while (!serial_is_transmit_ready(port))
    ;

  outb(SERIAL_DATA_REGISTER(port), c);
}

/**
 * @copydoc serial_write
 */
void serial_write(uint16_t port, unsigned char *str, size_t strlen)
{
  unsigned char *c = str;
  while (c != NULL && (size_t)(c - str) < strlen && *c != '\0')
  {
    serial_put(port, *c++);
  }
}
