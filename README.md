# ErOS: A minimal OS for personal education use
This is a toy OS I make in order to better understand the concepts of operating system. It's based on Intel x86 architecture (to be exact, qemu-system-i386).
The follwing tools are necessary for running the OS:
- nasm
- GNU toolchain for C (gcc, ld, make etc.)
- qemu

## To run the OS:
### 1. Make a drive folder and create a disk image using qemu-img
```
mkdir drive && cd drive
qemu-img create disk.img 1M
```
### 2. Write zeros to disk image
For some reason qemu doesn't acquire space even if you specify the disk size. So do it manually.
```
dd if=/dev/zeros of=disk.img bs=512 count=100
```
### 3. Compile and boot
```
make bootloader
make kernel
make boot
```
