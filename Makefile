# Compiler and tools
AS = nasm
ASFLAGS = -f elf32
CC = i686-elf-gcc
LD = i686-elf-ld
CFLAGS = -ffreestanding -O0 -nostdlib -m32 -Ikernel
LDFLAGS = -T linker.ld -m elf_i386

# Directories
BUILD_DIR = build
ISO_DIR = iso

# Start object file
START_SRC = start.asm
START_OBJ = $(BUILD_DIR)/start.o

# Interrupt handling source files
INT_ASM_SRC = kernel/interrupts/interrupt_asm_init.asm
INT_C_SRC = kernel/handlers/exception_handler.c

# Interrupt handling object files
INT_ASM_OBJ = $(BUILD_DIR)/interrupts/interrupt_asm_init.o
INT_C_OBJ = $(BUILD_DIR)/handlers/exception_handler.o

# Kernel source files
KERNEL_SRC = kernel/kernel.c
UTIL_SRC = $(wildcard kernel/util/*.c)
TERM_SRC = $(wildcard kernel/terminal/*.c)
PORT_IO_SRC = $(wildcard kernel/port_io/*.c)
INT_SRC = $(wildcard kernel/interrupts/*.c)
GDT_SRC = $(wildcard kernel/gdt/*.c)

# Source and object file lists
SRC_FILES = $(KERNEL_SRC) $(UTIL_SRC) $(TERM_SRC) $(PORT_IO_SRC) $(INT_SRC) $(GDT_SRC)
OBJ_FILES = $(patsubst kernel/%, $(BUILD_DIR)/%, $(SRC_FILES:.c=.o)) $(INT_ASM_OBJ) $(INT_C_OBJ) $(START_OBJ)


# Kernel binary output
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

all: $(ISO_DIR)/myos.iso

# Ensure the build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile exception handler first
$(INT_C_OBJ): $(INT_C_SRC) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile interrupt assembly file after the C object is ready
$(INT_ASM_OBJ): $(INT_ASM_SRC) $(INT_C_OBJ) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Compile start.asm separately
$(START_OBJ): $(START_SRC) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

# Compile all other C files into object files
$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Link the kernel binary
$(KERNEL_BIN): $(START_OBJ) $(OBJ_FILES) $(INT_ASM_OBJ) $(INT_C_OBJ)
	@echo "Linking kernel with: $^"
	$(LD) $(LDFLAGS) -o $@ $^


# Create the bootable ISO image
$(ISO_DIR)/myos.iso: $(KERNEL_BIN) grub/grub.cfg
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin
	cp grub/grub.cfg $(ISO_DIR)/boot/grub/
	wsl grub-mkrescue -o $@ $(ISO_DIR)

# Run the OS in QEMU
run: $(ISO_DIR)/myos.iso
	qemu-system-i386 -cdrom $(ISO_DIR)/myos.iso -no-reboot 

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)
