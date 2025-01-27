# DiamondOS

DiamondOS is a custom operating system project that is currently in development. The goal of this project is to create a simple, modular operating system that works with modern hardware, using low-level techniques and C programming. The OS is designed to be simple enough to understand while also providing the foundation for more complex features in the future.

## Features So Far
- **Basic Bootloader with GRUB**: The operating system boots via GRUB and is able to load a kernel.
- **GDT (Global Descriptor Table)**: Successfully implemented and initialized GDT with proper segment descriptors for code and data segments.
- **IDT (Interrupt Descriptor Table)**: Initial setup of interrupt handling. Hardware interrupts such as keyboard input are processed.
- **Interrupt Handlers**: Basic interrupt handlers for keyboard input have been implemented and tested. The OS correctly processes interrupt `0x21` (keyboard interrupt).
- **Terminal Output**: Implemented a simple terminal system that outputs text to the screen using VGA.
- **Stack Setup**: The OS is able to properly initialize and handle stack operations without crashes.
- **Multitasking Foundations**: Future plans include implementing basic task switching for a multi-tasking environment.

## Toolchain
The development of DiamondOS uses a variety of tools and libraries:

- **GCC (i686-elf-gcc)**: Used as the cross-compiler for compiling the kernel. It is configured to target a 32-bit architecture (`i686`).
- **GRUB**: Bootloader that loads the kernel image.
- **QEMU**: Used for running the OS in an emulator, which supports debugging and testing without needing to run on real hardware.
- **NASM**: Assembler used for writing low-level assembly code (such as for the bootloader and interrupt handling).
- **Makefile**: Used to automate the building process, including compiling, linking, and creating a bootable image.
- **GDB**: Used for debugging the OS during development.
- **Visual Studio Code**: Used for editing code with syntax highlighting and support for C, Assembly, and Makefile.

## Progress So Far
### **Initial Setup and Bootloader:**
- The bootloader loads the OS kernel from a GRUB-compatible bootable ISO.
- The kernel initializes basic system setup such as the stack and GDT.

### **Kernel Development:**
- **GDT**: The Global Descriptor Table was successfully initialized, allowing the kernel to operate in protected mode.
- **Interrupt Handling**: Basic interrupt handling has been implemented, including setting up the IDT and handling hardware interrupts such as keyboard input (IRQ 1).
- **Terminal Output**: Text-based terminal output is functional, allowing the OS to print messages to the screen.
- **Keyboard Input**: The keyboard interrupt is being processed, and characters typed on the keyboard are printed to the terminal.

### **Next Steps (What's Coming):**
- **Interrupt Service Routines (ISRs)**: Implement more advanced interrupt handling, including other hardware interrupts (e.g., timer interrupts).
- **Memory Management**: Add support for memory management, including page tables and virtual memory.
- **Multitasking**: Implement basic multitasking to allow the OS to run multiple tasks concurrently.
- **Filesystem**: Begin implementing a basic filesystem to allow file reading and writing.
- **Networking**: Start implementing networking capabilities to enable communication between machines or remote servers.
- **GUI (Graphical User Interface)**: Long-term goal to develop a basic GUI for user interaction.
- **More Drivers**: Implement drivers for additional hardware components (e.g., mouse, sound card, disk).

## How to Build and Run
### Requirements:
- **QEMU**: Emulator for testing the OS.
- **i686-elf-gcc**: Cross-compiler for compiling the OS.
- **NASM**: Assembler for low-level code.
- **Make**: Build automation tool.

### Build Process:
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/DiamondOS.git
   cd DiamondOS
