#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* VGA text-mode framebuffer starts at physical address 0xB8000 */
#define FRAMEBUFFER_MEMORY_OFFSET ((uint8_t *) 0xB8000)

/* Text-mode resolution */
#define FRAMEBUFFER_WIDTH  80
#define FRAMEBUFFER_HEIGHT 25

/* CRT controller I/O ports for cursor control (OSDev: Text Mode Cursor) */
#define CURSOR_PORT_CMD  0x03D4
#define CURSOR_PORT_DATA 0x03D5

/**
 * Terminal text framebuffer — 80×25 VGA text mode.
 *
 * Each character cell occupies 2 consecutive bytes in FRAMEBUFFER_MEMORY_OFFSET:
 *   byte[2*i]   — ASCII character code
 *   byte[2*i+1] — colour byte: bits[7:4] = background colour, bits[3:0] = foreground colour
 *
 * Colour indices: https://en.wikipedia.org/wiki/BIOS_color_attributes
 */

/**
 * framebuffer_write — Write a single character with colours to a cell.
 *
 * @param row  Row index, 0-based (0 = top)
 * @param col  Column index, 0-based (0 = left)
 * @param c    ASCII character to display
 * @param fg   Foreground (text) colour index [0, 15]
 * @param bg   Background colour index [0, 15]
 */
void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg);

/**
 * framebuffer_set_cursor — Move the hardware text cursor via CRT controller port I/O.
 *
 * @param r  Target row, 0-based
 * @param c  Target column, 0-based
 */
void framebuffer_set_cursor(uint8_t r, uint8_t c);

/**
 * framebuffer_clear — Blank the entire framebuffer.
 *
 * Sets every cell to character 0x00 and colour 0x07
 * (light-grey foreground on black background).
 */
void framebuffer_clear(void);

#endif
