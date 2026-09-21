#ifndef _PORTIO_H
#define _PORTIO_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * out — Send a byte to an x86 I/O port.
 * Uses the x86 `outb` instruction (Port-Mapped I/O).
 *
 * @param port The I/O port address to write to
 * @param data The byte value to send
 */
void out(uint16_t port, uint8_t data);

/**
 * in — Read a byte from an x86 I/O port.
 * Uses the x86 `inb` instruction (Port-Mapped I/O).
 *
 * @param port The I/O port address to read from
 * @return The byte received from the port
 */
uint8_t in(uint16_t port);

#endif
