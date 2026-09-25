#include <stdint.h>
#include <stdbool.h>
#include "header/cpu/gdt.h"
#include "header/cpu/interrupt.h"
#include "header/cpu/idt.h"
#include "header/kernel-entrypoint.h"
#include "header/text/framebuffer.h"
#include "header/driver/keyboard.h"

void kernel_setup(void) {
    load_gdt(&_gdt_gdtr);   // punya Adnan
    pic_remap();            // IRQ 0-15 pindah ke 0x20-0x2F, semua masked
    initialize_idt();       // isi IDT, lidt, sti

    framebuffer_clear();    // punya Alek
    framebuffer_set_cursor(0, 0);

    __asm__("int $0x4");    // tes IDT: harus masuk main_interrupt_handler, bukan triple fault

    activate_keyboard_interrupt();  // buka mask IRQ1, sisanya tetap masked
    keyboard_state_activate();

    int row = 0, col = 0;
    while (true) {
        char c;
        get_keyboard_buffer(&c);
        if (c) {
            if (c == '\b') {
                if (col > 0) {
                    col--;
                    framebuffer_write(row, col, '\0', 0xF, 0);
                }
            } else if (c == '\n') {
                col = 0;
                row++;
            } else {
                framebuffer_write(row, col, c, 0xF, 0);
                col++;
            }

            if (col >= FRAMEBUFFER_WIDTH) {
                col = 0;
                row++;
            }
            if (row >= FRAMEBUFFER_HEIGHT) {
                row = 0; // Simple wrap-around
                framebuffer_clear();
            }
            
            framebuffer_set_cursor(row, col);
        }
    }
}
