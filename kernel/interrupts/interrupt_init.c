
#include "interrupt_init.h"


void debug_idt_entries() {
    for (uint8_t i = 0; i < 32; i++) {
        terminal_print("IDT Entry ");
        terminal_print_hex(i);
        terminal_print(": ISR=");
        terminal_print_hex((uint32_t)(idt[i].isr_high << 16) | idt[i].isr_low);
        terminal_print("\n");
    }
}



void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}


// Basic US QWERTY scancode-to-ASCII converter (partial example)


void default_interrupt_handler(interrupt_frame_t* frame) {
    // Read the keyboard scancode from I/O port 0x60
    uint8_t scancode = inb(0x60);

    // Send End of Interrupt (EOI) to the PIC
    outb(0x20, 0x20);

    // Print a basic debug message (avoid floating-point operations)
    //terminal_print("Interrupt triggered!");
}

void pic_remap(int offset1, int offset2) {
    // Save the original interrupt masks
    uint8_t mask1 = inb(0x21);  // Master PIC mask
    uint8_t mask2 = inb(0xA1);  // Slave PIC mask

    // Start the initialization sequence (cascade mode)
    outb(0x20, 0x11);  // Start init for Master PIC
    outb(0xA0, 0x11);  // Start init for Slave PIC

    // Set vector offsets for remapping
    outb(0x21, offset1);  // Remap Master PIC to offset1 (e.g., 0x20)
    outb(0xA1, offset2);  // Remap Slave PIC to offset2 (e.g., 0x28)

    // Tell Master PIC about the Slave PIC at IRQ2 (cascade mode)
    outb(0x21, 0x04);

    // Tell Slave PIC its cascade identity
    outb(0xA1, 0x02);

    // Set both PICs to 8086/88 (x86) mode
    outb(0x21, 0x01);  // Master PIC
    outb(0xA1, 0x01);  // Slave PIC

    // Restore the saved masks (unmask only what you want to handle)
    outb(0x21, mask1);  // Restore Master PIC mask
    outb(0xA1, mask2);  // Restore Slave PIC mask
}


void init_keyboard() {
    // Enable the keyboard by writing to port 0x64
    // Command: 0xAE enables the keyboard
    outb(0x64, 0xAE);

    // Clear any pending output in the keyboard buffer
    while (inb(0x64) & 0x01) {
        inb(0x60);
    }
}

void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    pic_remap(0x20, 0x28);  // Remap the PIC to start at interrupt 0x20
    init_keyboard();
    // Set up the IDT for all vectors
    for (uint16_t vector = 0; vector < 256; vector++) {
        if (vector == 0x21) {
            // Set specific handler for IRQ1 (keyboard)
            idt_set_descriptor(vector, (void*)keyboard_interrupt_handler, 0x8E);
        } else {
            // Set a default handler for other interrupts
            idt_set_descriptor(vector, (void*)default_interrupt_handler, 0x8E);
        }
        vectors[vector] = true;  // Mark the vector as active
    }
    outb(0x21, 0xFD); // Master PIC: Unmask IRQ1 (bit 1) and IRQ2 (bit 2, needed for Slave PIC)
    //outb(0xA1, 0xEF); // Slave PIC: Unmask IRQ12 (bit 4)
    // Load the IDT into the CPU
    __asm__ volatile ("lidt %0" : : "m"(idtr));  // Load the new IDT
    __asm__ volatile ("sti");  // Set the interrupt flag to enable interrupts
}
