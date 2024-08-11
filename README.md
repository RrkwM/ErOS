# ErOS: A minimal OS for personal education use
This is a toy OS I make in order to better understand the concepts of operating system. It's based on Intel x86 architecture (to be exact, qemu-system-i386).
The follwing tools are necessary for running the OS:
- nasm
- GNU toolchain for C (gcc, ld, make etc.)
- qemu

## To run the OS:
Simply run:
```
make
make boot
```
If you're in terminal environment without GUI, you can use `boot-nogui` target
```
make boot-nogui
```
