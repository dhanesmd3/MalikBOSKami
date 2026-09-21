#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "header/cpu/portio.h"

/**
 * x86 Port-Mapped I/O wrappers.
 *
 * `outb` sends one byte to a port via the x86 OUT instruction.
 * `inb`  reads one byte from a port via the x86 IN instruction.
 *
 * Operand constraints:
 *   "a"  — value goes into AL (8-bit accumulator)
 *   "Nd" — port: use immediate 8-bit const if possible, else DX register
 *   "=a" — output written to AL
 */

void out(uint16_t port, uint8_t data) {
    __asm__ volatile(
        "outb %0, %1"
        : /* no output */
        : "a"(data), "Nd"(port)
    );
}

uint8_t in(uint16_t port) {
    uint8_t result;
    __asm__ volatile(
        "inb %1, %0"
        : "=a"(result)
        : "Nd"(port)
    );
    return result;
}
