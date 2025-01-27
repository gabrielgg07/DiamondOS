#include <stdint.h>

#include "gdt_init.h"

struct gdt_entry gdt[GDT_ENTRY_COUNT];
struct gdt_ptr gdt_pointer;

// Function to set a GDT entry
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[index].base_low = (uint16_t)(base & 0xFFFF);
    gdt[index].base_middle = (uint8_t)((base >> 16) & 0xFF);
    gdt[index].base_high = (uint8_t)((base >> 24) & 0xFF);

    gdt[index].limit_low = (uint16_t)(limit & 0xFFFF);
    gdt[index].granularity = (uint8_t)((limit >> 16) & 0x0F);

    gdt[index].granularity |= (granularity & 0xF0);
    gdt[index].access = access;
}

// Function to initialize the GDT
void init_gdt() {
    gdt_pointer.limit = (sizeof(struct gdt_entry) * GDT_ENTRY_COUNT) - 1;
    gdt_pointer.base = (uint32_t)&gdt;

    // Null descriptor (entry 0)
    set_gdt_entry(0, 0, 0, 0, 0);

    // Code segment (entry 1): 4GB limit, base 0x0, executable, readable, ring 0, present
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data segment (entry 2): 4GB limit, base 0x0, writable, ring 0, present
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load the GDT
    asm volatile ("lgdt (%0)" :: "r"(&gdt_pointer));

    // Update segment registers
    asm volatile (
        "mov $0x10, %ax\n"  // Load data segment selector (0x10)
        "mov %ax, %ds\n"
        "mov %ax, %es\n"
        "mov %ax, %fs\n"
        "mov %ax, %gs\n"
        "mov %ax, %ss\n"
        "ljmp $0x08, $.flush\n"  // Far jump to load CS (0x08)
        ".flush:\n"
    );
}
