; A boot sector that boots a C kernel in 32-bit protected mode

[org 0x7c00]

KERNEL_OFFSET equ 0x1000       ; Memory offset to load our kernel

mov [BOOT_DRIVE], dl           ; Store the boot drive number in BOOT_DRIVE
mov bp, 0x9000                 ; Set up the stack
mov sp, bp

mov bx, MSG_REAL_MODE          ; Load message: starting from 16-bit real mode
call print_string              ; Print the message

call load_kernel               ; Load the kernel from the boot disk
call switch_to_pm              ; Switch to protected mode

jmp $                          ; Infinite loop (halt)

; Include our useful routines
%include "print_string.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "print_string_pm.asm"
%include "switch_to_pm.asm"

[bits 16]

; Load kernel into memory
load_kernel:
    mov bx, MSG_LOAD_KERNEL    ; Print message: loading kernel
    call print_string          ; Print the message
    
    mov bx, KERNEL_OFFSET      ; Set parameters for disk_load
    mov dh, 15                 ; Number of sectors to read (excluding boot sector)
    mov dl, [BOOT_DRIVE]       ; Boot drive number
    call print_string
    call disk_load             ; Load the kernel to KERNEL_OFFSET

    ret                        ; Return from the function

[bits 32]

; This is where we arrive after switching to and initializing protected mode
BEGIN_PM:
    mov ebx, MSG_PROT_MODE     ; Load message: successfully in protected mode
    call print_string_pm       ; Print the message
    
    call KERNEL_OFFSET         ; Jump to the address of the loaded kernel code

    jmp $                      ; Infinite loop (halt)

; Global variables
BOOT_DRIVE db 0
MSG_REAL_MODE db "Started in 16-bit Real Mode...", 0
MSG_PROT_MODE db "Successfully landed in 32-bit Protected Mode.", 0
MSG_LOAD_KERNEL db "Loading kernel into memory...", 0

; Bootsector padding
times 510 - ($ - $$) db 0
dw 0xaa55
