; print_string function
print_string:
    pusha               ; Save all general-purpose registers
.next_char:
    mov al, [bx]        ; Load byte from address in BX into AL
    cmp al, 0           ; Compare AL with null terminator
    je .done            ; Jump to .done if zero (end of string)
    mov ah, 0x0E        ; BIOS teletype function
    int 0x10            ; Print character in BX
    inc bx              ; Move to the next character
    jmp .next_char      ; Repeat loop
.done:
    popa                ; Restore all general-purpose registers
    ret                 ; Return to the caller