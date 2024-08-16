;Generic Interrupt Handler
;
extern interrupt_handler

%macro no_error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	pushad
	mov ebp, esp
	push	dword 0                     ; push 0 as error code
	push	dword %1
	call    interrupt_handler            ; push the interrupt number
	add     esp, 8
	popad
	iret
%endmacro

%macro error_code_interrupt_handler 1
global interrupt_handler_%1
interrupt_handler_%1:
	pushad
	mov ebp, esp
	push    dword %1                    ; push the interrupt number
	call    interrupt_handler            ; push the interrupt number
	popad
	add     esp, 4
	iret
%endmacro

	no_error_code_interrupt_handler 33

;   error_code_interrupt_handler              7       ; create handler for interrupt 7