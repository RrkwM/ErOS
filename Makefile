#define some variables
ASM_SRC_DIR = asm
ASM_BOOT_SRC_DIR = asm_boot
C_SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DRIVE_DIR = drive

#getting files using wildcard
ASM_SRC = $(wildcard $(ASM_SRC_DIR)/*.asm)
ASM_BOOT_SRC = $(wildcard $(ASM_BOOT_SRC_DIR)/*.asm)
C_SRC = $(wildcard $(C_SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(ASM_SRC_DIR)/%.asm,$(BUILD_DIR)/%_asm.o,$(ASM_SRC)) \
			$(patsubst $(C_SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(C_SRC))
			

all: prepare bootloader kernel

#preparing for the directory
prepare:
	@echo "Current working directory: $(CURDIR)/"
	@echo "Building..."
	mkdir -p build drive
	qemu-img create $(DRIVE_DIR)/disk.img 1M
	dd if=/dev/zero of=$(DRIVE_DIR)/disk.img bs=512 count=100

#rules for making boot loader, from asm_boot
bootloader: $(BUILD_DIR)/bootloader.bin
	dd if=$(BUILD_DIR)/bootloader.bin of=$(DRIVE_DIR)/disk.img bs=512 seek=0 conv=notrunc

$(BUILD_DIR)/bootloader.bin: $(ASM_BOOT_SRC_DIR)/bootloader.asm
	nasm -f bin -o $(BUILD_DIR)/bootloader.bin $(ASM_BOOT_SRC_DIR)/bootloader.asm

#rules for making kernel, from src and asm
kernel: $(BUILD_DIR)/kernel.bin
	dd if=$(BUILD_DIR)/kernel.bin of=$(DRIVE_DIR)/disk.img bs=512 seek=1 conv=notrunc

# link into elf and obj copy to raw binary

$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/kernel.elf
	objcopy -O binary $(BUILD_DIR)/kernel.elf $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.elf: $(OBJ_FILES) $(BUILD_DIR)/kernel_entry_asm.o
	ld -m elf_i386 -o $(BUILD_DIR)/kernel.elf -Ttext 0x1000 $(BUILD_DIR)/kernel_entry_asm.o $(OBJ_FILES)

# compile assembly source with debug info
$(BUILD_DIR)/%_asm.o: $(ASM_SRC_DIR)/%.asm
	nasm -f elf -g -F dwarf -o $@ $<

# compile C source with debug info
$(BUILD_DIR)/%.o: $(C_SRC_DIR)/%.c
	gcc -g -O0 -I$(INCLUDE_DIR) -m32 -fno-pic -ffreestanding -c $< -o $@

boot:
	qemu-system-i386 -m 16M -drive file=$(DRIVE_DIR)/disk.img,format=raw

boot-nogui:
	qemu-system-i386 -m 16M -drive file=$(DRIVE_DIR)/disk.img,format=raw -vnc :1

debug:
	qemu-system-i386 -m 16M -drive file=$(DRIVE_DIR)/disk.img,format=raw -vnc :1 -d int -no-shutdown -no-reboot -s -S

clean:
	rm -rf $(BUILD_DIR) $(DRIVE_DIR)
