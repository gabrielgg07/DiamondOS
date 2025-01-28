#ifndef INTERRUPT_INIT_H
#define INTERRUPT_INIT_H

#include <stdint.h>
#include <stdbool.h>
#include "../port_io/port_io.h"
#include "../terminal/terminal.h"
#include "../drivers/keyboard/keyboard.h"

#define IDT_SIZE 256
#define IDT_MAX_DESCRIPTORS 256


typedef struct {
    uint16_t    isr_low;      // The lower 16 bits of the ISR's address
    uint16_t    kernel_cs;    // The GDT segment selector that the CPU will load into CS before calling the ISR
    uint8_t     reserved;     // Set to zero
    uint8_t     attributes;   // Type and attributes; see the IDT page
    uint16_t    isr_high;     // The higher 16 bits of the ISR's address
} __attribute__((packed)) idt_entry_t;


typedef struct {
	uint16_t	limit;
	uint32_t	base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10)))
static idt_entry_t idt[IDT_SIZE]; // Create an array of IDT entries; aligned for performance

static idtr_t idtr;


static bool vectors[IDT_MAX_DESCRIPTORS];

extern void* isr_stub_table[];

extern void keyboard_interrupt_handler();



void idt_set_descriptor(uint8_t vector,  void* isr, uint8_t flags);

void idt_init();



#endif // IDT_H


