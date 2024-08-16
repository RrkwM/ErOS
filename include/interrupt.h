#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "types.h"
#include "x86.h"

// Port addresses for the Master and Slave PICs
#define MASTER_PIC_COMMAND  0x20  // Command port of the Master PIC
#define MASTER_PIC_DATA     0x21  // Data port of the Master PIC
#define SLAVE_PIC_COMMAND   0xA0  // Command port of the Slave PIC
#define SLAVE_PIC_DATA      0xA1  // Data port of the Slave PIC

// End-of-Interrupt (EOI) command for PICs
#define PIC_EOI             0x20  // End of interrupt command to signal PIC that interrupt service is complete

// OCW3 (Operation Control Word 3) register definitions
#define PIC_READ_IRR        0x0A  // Read Interrupt Request Register (IRR) - shows IRQs that are requesting service
#define PIC_READ_ISR        0x0B  // Read In-Service Register (ISR) - shows IRQs that are currently being serviced

// ICW1 (Initialization Control Word 1) definitions
#define ICW1_ICW4          0x01  // Indicates that ICW4 (Initialization Control Word 4) will be used
#define ICW1_SINGLE        0x02  // Single (non-cascade) mode
#define ICW1_INTERVAL4     0x04  // Call address interval 4 (8 IRQs per PIC)
#define ICW1_LEVEL         0x08  // Level triggered mode (as opposed to edge triggered)
#define ICW1_INIT          0x10  // Initialization command (must be set during initialization)

// ICW4 (Initialization Control Word 4) definitions
#define ICW4_8086          0x01  // Set PIC to 8086/88 mode (instead of 8080 mode)
#define ICW4_AUTO          0x02  // Automatic End-of-Interrupt (EOI) mode
#define ICW4_BUF_SLAVE     0x08  // Buffered mode for Slave PIC
#define ICW4_BUF_MASTER    0x0C  // Buffered mode for Master PIC
#define ICW4_SFNM          0x10  // Special Fully Nested Mode (not used)

// ICW2 (Initialization Control Word 2) definitions
#define MASTER_PIC_VECTOR_OFFSET  0x20  // Vector offset for interrupts on the Master PIC
#define SLAVE_PIC_VECTOR_OFFSET   0x28  // Vector offset for interrupts on the Slave PIC

// ICW3 (Initialization Control Word 3) definitions
#define MASTER_PIC_ICW3       0x04  // Master PIC is connected to Slave PIC on IRQ2
#define SLAVE_PIC_ICW3        0x02  // Slave PIC cascade identity (connected to Master PIC IRQ2)

// OCW2 (Operation Control Word 2) definitions
#define PIC_OCW2_NORMAL_MODE  0x01  // Normal operation mode (used to send specific commands to PIC)

#define IDT_ADDR 0x00F00000 //Last 1M space
#define IDT_SIZE 256    //Total length of IDT
#define IDT_LIMIT 0xFFFF // A single IDT entry is 64bit.

// Interrupt service routine pointer
typedef void (*isr_t)(void);
typedef enum {
    TASK_GATE = 0x5,         // 0b0101
    INTERRUPT_GATE_16 = 0x6, // 0b0110
    TRAP_GATE_16 = 0x7,      // 0b0111
    INTERRUPT_GATE_32 = 0xE, // 0b1110
    TRAP_GATE_32 = 0xF       // 0b1111
} interrupt_gate_type;

// The structure of the gates are basically the same.
typedef struct __attribute__((packed)) {
    uint16_t offset_low;       // Offset 0..15 (Lower part of the offset address)
    uint16_t selector;         // Selector (Segment selector for the function)
    uint8_t  reserved;         // Reserved bits (must be 0)
    uint8_t  type : 4;         // Gate Type (4 bits)
    uint8_t  s : 1;            // Storage Segment (must be 0 for gates)
    uint8_t  dpl : 2;          // Descriptor Privilege Level (2 bits)
    uint8_t  p : 1;            // Present (1 bit)
    uint16_t offset_high;      // Offset 16..31 (Higher part of the offset address)
} idt_entry;

// struct that describes the whole IDT for loading into IDTR
typedef struct __attribute__((packed)) {
    uint16_t limit;        // IDT limit
    uint32_t base;         // IDT base
} idt_desc;

//general interfaces
void kinit_interrupt();
void idt_set_entry(int, uint32_t, uint16_t, interrupt_gate_type, uint8_t);
void isr_install_handler(int, isr_t);
void isr_uninstall_handler(int);
void load_idt(idt_desc *);

//internal utils
void pic_init();
void pic_send_eoi(uint8_t);
void pic_set_mask(uint8_t);
void pic_clear_mask(uint8_t);
void pic_remap(int, int);
uint16_t pic_get_irr(void);
uint16_t pic_get_isr(void);

isr_t interrupt_handler(uint32_t, uint32_t);

extern isr_t interrupt_handler_33();
isr_t keyboard_handler();

#endif