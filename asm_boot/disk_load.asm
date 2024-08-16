; Load DH sectors to ES:BX from drive DL
disk_load:
    push dx                  ; Save DX on the stack to preserve its value

    mov ah, 0x02             ; BIOS function 0x02: Read sectors
    mov al, dh               ; Number of sectors to read
    mov ch, 0x00             ; Cylinder 0
    mov dh, 0x00             ; Head 0
    mov cl, 0x02             ; Start reading from sector 2 (after the boot sector)

    int 0x13                 ; Call BIOS interrupt 0x13 (disk services)

    jc disk_error1           ; Jump to disk_error if carry flag is set (error occurred)

    pop dx                   ; Restore DX from the stack

    cmp dh, al               ; Compare sectors read (AL) with sectors expected (DH)
    jne disk_error2           ; Jump to disk_error if they do not match

    ret                      ; Return from the function

disk_error1:
    mov bx, DISK_ERROR_MSG1   ; Load address of the error message
    call print_string        ; Call function to print the error message
    jmp $                    ; Infinite loop to halt execution

disk_error2:
    mov bx, DISK_ERROR_MSG2   ; Load address of the error message
    call print_string        ; Call function to print the error message
    jmp $                    ; Infinite loop to halt execution

; Variables
DISK_ERROR_MSG1 db "Disk read error1!", 0
DISK_ERROR_MSG2 db "Disk read error2!", 0