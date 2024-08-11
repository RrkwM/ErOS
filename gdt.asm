; GDT
gdt_start:

    ; The mandatory null descriptor
    gdt_null:
        dd 0x0          ; Define double word (4 bytes)
        dd 0x0

    ; The code segment descriptor
    gdt_code:
        ; Base = 0x0, Limit = 0xfffff
        ; 1st flags: (present) 1, (privilege) 00, (descriptor type) 1 -> 1001b
        ; Type flags: (code) 1, (conforming) 0, (readable) 1, (accessed) 0 -> 1010b
        ; 2nd flags: (granularity) 1, (32-bit default) 1, (64-bit seg) 0, (AVL) 0 -> 1100b
        
        dw 0xffff        ; Limit (bits 0-15)
        dw 0x0           ; Base (bits 0-15)
        db 0x0           ; Base (bits 16-23)
        db 10011010b     ; 1st flags, type flags
        db 11001111b     ; 2nd flags, Limit (bits 16-19)
        db 0x0           ; Base (bits 24-31)

    ; The data segment descriptor
    gdt_data:
        ; Same as code segment except for the type flags:
        ; Type flags: (code) 0, (expand down) 0, (writable) 1, (accessed) 0 -> 0010b
        
        dw 0xffff        ; Limit (bits 0-15)
        dw 0x0           ; Base (bits 0-15)
        db 0x0           ; Base (bits 16-23)
        db 10010010b     ; 1st flags, type flags
        db 11001111b     ; 2nd flags, Limit (bits 16-19)
        db 0x0           ; Base (bits 24-31)

gdt_end:                  ; The end of the GDT
    ; The reason for placing a label here is to calculate the size of the GDT
    ; for the GDT descriptor (below).

; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of the GDT (always less one of the true size)
    dd gdt_start               ; Start address of the GDT

; Define some handy constants for the GDT segment descriptor offsets,
; which are what segment registers must contain when in protected mode.
; For example, when DS = 0x10 in PM, the CPU knows to use the segment described
; at offset 0x10 (16 bytes) in our GDT, which is the DATA segment.
; (0x0 -> NULL; 0x08 -> CODE; 0x10 -> DATA)

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
