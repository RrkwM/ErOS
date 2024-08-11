bootloader: bootloader.asm
	nasm -f bin -o bootloader.bin bootloader.asm
	dd if=bootloader.bin of=../drive/disk.img bs=512 seek=0 conv=notrunc

kernel: kernel.c kernel_entry.asm
	nasm -f elf -o kernel_entry.o kernel_entry.asm
	gcc -m32 -fno-pic -ffreestanding -c kernel.c -o kernel.o
	ld -m elf_i386 -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary
	dd if=kernel.bin of=../drive/disk.img bs=512 seek=1 conv=notrunc

boot:
	qemu-system-i386 -drive file=../drive/disk.img,format=raw

clean:
	rm -f *.bin *.o
