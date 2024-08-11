#define some variables
ASM_SRC_DIR = asm
C_SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DRIVE_DIR = drive

#getting files using wildcard
ASM_SRC = $(wildcard $(ASM_SRC_DIR)/*.asm)
C_SRC = $(wildcard $(C_SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(C_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))

all: prepare bootloader kernel

#preparing for the directory
prepare:
	@echo "Current working directory: $(CURDIR)/"
	@echo "Building..."
	mkdir -p build drive
	qemu-img create $(DRIVE_DIR)/disk.img 1M
	dd if=/dev/zero of=$(DRIVE_DIR)/disk.img bs=512 count=100

bootloader: $(BUILD_DIR)/bootloader.bin
	dd if=$(BUILD_DIR)/bootloader.bin of=$(DRIVE_DIR)/disk.img bs=512 seek=0 conv=notrunc

$(BUILD_DIR)/bootloader.bin: $(ASM_SRC_DIR)/bootloader.asm
	nasm -f bin -o $(BUILD_DIR)/bootloader.bin $(ASM_SRC_DIR)/bootloader.asm

kernel: $(BUILD_DIR)/kernel.bin
	dd if=$(BUILD_DIR)/kernel.bin of=$(DRIVE_DIR)/disk.img bs=512 seek=1 conv=notrunc

$(BUILD_DIR)/kernel.bin: $(OBJ_FILES) $(ASM_SRC_DIR)/kernel_entry.asm
	nasm -f elf -o $(BUILD_DIR)/kernel_entry.o $(ASM_SRC_DIR)/kernel_entry.asm
	ld -m elf_i386 -o $(BUILD_DIR)/kernel.bin -Ttext 0x1000 $(OBJ_FILES) --oformat binary

$(BUILD_DIR)/%.o: $(C_SRC_DIR)/%.c
	gcc -I$(INCLUDE_DIR) -m32 -fno-pic -ffreestanding -c $< -o $@

boot:
	qemu-system-i386 -drive file=$(DRIVE_DIR)/disk.img,format=raw

boot-nogui:
	qemu-system-i386 -drive file=$(DRIVE_DIR)/disk.img,format=raw -vnc :1

clean:
	rm -rf $(BUILD_DIR) $(DRIVE_DIR)
