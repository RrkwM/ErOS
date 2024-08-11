[bits 16]

; Switch to protected mode
switch_to_pm:
    cli                    ; Disable interrupts to prevent them from firing
                           ; before the protected mode interrupt vector is set up.
    lgdt [gdt_descriptor]  ; Load the Global Descriptor Table (GDT), which defines
                           ; the protected mode segments (e.g., for code and data).
    
    mov eax, cr0            ; Load the value of the CR0 control register into EAX.
    or eax, 0x1             ; Set the PE (Protection Enable) bit (bit 0) of CR0.
    mov cr0, eax            ; Write the updated value back to CR0.
    
    jmp CODE_SEG:init_pm    ; Far jump to the 32-bit code segment (init_pm).
                           ; This also flushes the CPU's instruction cache,
                           ; which may contain pre-fetched and real-mode decoded instructions.

[bits 32]

; Initialize registers and the stack once in protected mode.
init_pm:
    mov ax, DATA_SEG        ; Load the data segment selector into AX.
    mov ds, ax              ; Set DS to the new data segment selector.
    mov ss, ax              ; Set SS to the new data segment selector.
    mov es, ax              ; Set ES to the new data segment selector.
    mov fs, ax              ; Set FS to the new data segment selector.
    mov gs, ax              ; Set GS to the new data segment selector.
    
    mov ebp, 0x90000        ; Set up the base of the stack.
    mov esp, ebp            ; Update ESP to point to the top of the stack.

    call BEGIN_PM           ; Call a well-known label to continue execution.
