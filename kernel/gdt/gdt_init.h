#ifndef GDT_INIT_H
#define GDT_INIT_H

#include <stdint.h>

//set number of gdt entries
#define GDT_ENTRY_COUNT 3

// GDT Entry structure
struct gdt_entry {
    uint16_t limit_low;     // Lower 16 bits of the limit
    uint16_t base_low;      // Lower 16 bits of the base
    uint8_t  base_middle;   // Next 8 bits of the base
    uint8_t  access;        // Access flags
    uint8_t  granularity;   // Granularity and limit high bits
    uint8_t  base_high;     // Last 8 bits of the base
} __attribute__((packed));


// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;    // Size of the GDT minus 1
    uint32_t base;     // Address of the first GDT entry
} __attribute__((packed));

// Declare the GDT and GDT pointer






//set GDT entry
void set_gdt_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity);

//initialize the gdt
void init_gdt();

#endif