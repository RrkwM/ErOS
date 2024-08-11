[bits 32]

; Define some constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; Prints a null-terminated string pointed to by EBX to the video memory
print_string_pm:
    pusha                   ; Save all general-purpose registers
    
    mov edx, VIDEO_MEMORY   ; Set EDX to the start of video memory
    
print_string_pm_loop:
    mov al, [ebx]           ; Load the character at [EBX] into AL
    mov ah, WHITE_ON_BLACK  ; Load the attribute (white on black) into AH
    
    cmp al, 0               ; Check if the character is NULL (end of string)
    je print_string_pm_done ; If NULL, jump to the done label
    
    mov [edx], ax           ; Store the character and attributes at current position
    add ebx, 1              ; Move to the next character in the string
    add edx, 2              ; Move to the next character cell in video memory
    
    jmp print_string_pm_loop ; Repeat the loop to print the next character

print_string_pm_done:
    popa                    ; Restore all general-purpose registers
    ret                     ; Return from the function
