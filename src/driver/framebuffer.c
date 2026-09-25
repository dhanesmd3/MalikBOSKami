#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "header/text/framebuffer.h"
#include "header/stdlib/string.h"
#include "header/cpu/portio.h"

/**
 * framebuffer_write — Write character 'c' with colours to cell (row, col).
 *
 * VGA text framebuffer memory layout (base = 0xB8000):
 *   Each cell is 2 bytes at offset (row * 80 + col) * 2:
 *     [+0] ASCII character byte
 *     [+1] colour attribute: (bg << 4) | (fg & 0x0F)
 *
 * Reference: https://wiki.osdev.org/Text_UI
 *
 * Why multiply offset by 2?
 *   The FRAMEBUFFER_MEMORY_OFFSET pointer is uint8_t*, so subscript [n]
 *   advances n bytes. Each cell occupies 2 bytes, hence *2.
 */
void framebuffer_write(uint8_t row, uint8_t col, char c, uint8_t fg, uint8_t bg) {
    uint16_t offset = (uint16_t)row * FRAMEBUFFER_WIDTH + col;
    FRAMEBUFFER_MEMORY_OFFSET[2 * offset]     = (uint8_t)c;
    FRAMEBUFFER_MEMORY_OFFSET[2 * offset + 1] = (uint8_t)((bg << 4) | (fg & 0x0F));
}

/**
 * framebuffer_set_cursor — Reposition the blinking text cursor.
 *
 * The VGA CRT controller uses a two-register indirect scheme:
 *   1. Write the register index to CURSOR_PORT_CMD (0x3D4).
 *   2. Write the data byte   to CURSOR_PORT_DATA (0x3D5).
 *
 * Register 0x0F = Cursor Location Low  (bits 7:0 of linear position)
 * Register 0x0E = Cursor Location High (bits 15:8 of linear position)
 * Linear position = row * 80 + col
 *
 * Reference: https://wiki.osdev.org/Text_Mode_Cursor
 */
void framebuffer_set_cursor(uint8_t r, uint8_t c) {
    uint16_t pos = (uint16_t)r * FRAMEBUFFER_WIDTH + c;
    out(CURSOR_PORT_CMD,  0x0F);
    out(CURSOR_PORT_DATA, (uint8_t)(pos & 0xFF));
    out(CURSOR_PORT_CMD,  0x0E);
    out(CURSOR_PORT_DATA, (uint8_t)((pos >> 8) & 0xFF));
}

/**
 * framebuffer_clear — Blank all 80×25 cells.
 *
 * Writes 0x00 (null character) and 0x07 (light-grey on black) to every byte
 * of the framebuffer.  memset over the full 80*25*2 = 4000 byte region works
 * because both the character byte (0x00) and the colour byte (0x07 != 0x00)
 * need separate initialisation; we do two passes via a loop rather than a single
 * memset to keep the colour byte correct.
 *
 * Alternative: memset the whole region to 0x00 first, then fill colour bytes.
 * Current approach: one loop, explicit per-cell write — clear and readable.
 */
void framebuffer_clear(void) {
    for (uint8_t row = 0; row < FRAMEBUFFER_HEIGHT; row++) {
        for (uint8_t col = 0; col < FRAMEBUFFER_WIDTH; col++) {
            framebuffer_write(row, col, '\0', 0x7, 0x0);
        }
    }
}
