#include <stdint.h>
#include <stdbool.h>
#include "header/cpu/gdt.h"
#include "header/cpu/interrupt.h"
#include "header/cpu/idt.h"
#include "header/kernel-entrypoint.h"
#include "header/driver/framebuffer.h"

void kernel_setup(void) {
    load_gdt(&_gdt_gdtr);   // punya Adnan
    pic_remap();            // IRQ 0-15 pindah ke 0x20-0x2F, semua masked
    initialize_idt();       // isi IDT, lidt, sti

    framebuffer_clear();    // punya Alek, comment dulu kalau belum ada
    framebuffer_set_cursor(0, 0);

    __asm__("int $0x4");    // tes IDT: harus masuk main_interrupt_handler, bukan triple fault

    while (true);
}
