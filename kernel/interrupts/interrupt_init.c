
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



void idt_set_descriptor(uint8_t vector, uintptr_t isr, uint8_t flags) {
    idt_entry_t* descriptor = &idt[vector];

    descriptor->isr_low        = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs      = 0x08; // this value can be whatever offset your kernel code selector is in your GDT
    descriptor->attributes     = flags;
    descriptor->isr_high       = (uint32_t)isr >> 16;
    descriptor->reserved       = 0;
}


// Basic US QWERTY scancode-to-ASCII converter (partial example)


void default_interrupt_handler(void) {
    // Read the keyboard scancode from I/O port 0x60
    // Send End of Interrupt (EOI) to the master PIC (IRQ1 = keyboard)
    outb(0x20, 0x20);
    terminal_print("Interrupt Called!");
    while(1);


}

void pic_remap(int offset1, int offset2) {
    outb(0x20, 0x11);  // Initialize PIC1
    outb(0xA0, 0x11);  // Initialize PIC2
    outb(0x21, offset1);  // Set vector offset for PIC1 (e.g., 0x20)
    outb(0xA1, offset2);  // Set vector offset for PIC2 (e.g., 0x28)
    outb(0x21, 0x04);  // Tell PIC1 about PIC2 at IRQ2 (0000 0100)
    outb(0xA1, 0x02);  // Tell PIC2 its cascade identity (0000 0010)
    outb(0x21, 0x01);  // 8086 mode
    outb(0xA1, 0x01);  // 8086 mode
    outb(0x21, 0x0);   // Unmask all interrupts
    outb(0xA1, 0x0);
}



void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
    pic_remap(0x20, 0x28);
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, (uintptr_t)default_interrupt_handler, 0x8E);
        vectors[vector] = true;
    }
    outb(0x21, 0xFD);  // Mask all IRQs on PIC1 (Master)

    //debug_idt_entries();
    //while(1);
    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}