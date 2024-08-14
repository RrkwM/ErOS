#include "interrupt.h"
#include "utils.h"

void kinit_interrupt(){
    struct idt_entry *idt = (struct idt_entry *)IDT_ADDR;
    idt_desc idtr;
    disable_interrupts();

    // Set all idt_entry
    for (int i = 0; i < IDT_SIZE; i++) {
        // using default ISR，selector 0x08（code seg selector），32-bit interrupt gate (0xE)
        idt_set_entry(i, (uint32_t)interrupt_handler, 0x08, INTERRUPT_GATE_32, 0);
    }
    idt_set_entry(33, (uint32_t)interrupt_handler_33, 0x08, INTERRUPT_GATE_32, 0);
    load_idt(&idtr);
    pic_init();
    enable_interrupts();
}
void idt_set_entry(int num, uint32_t base, uint16_t selector, interrupt_gate_type type, uint8_t dpl) {
    // get IDT address
    idt_entry *idt = (idt_entry *)IDT_ADDR;

    // calculate entry addr
    idt_entry *entry = &idt[num];
    entry->offset_low = base & 0xFFFF;
    entry->selector = selector;
    entry->reserved = 0;         // Must be 0
    entry->type = type;          // Gate type
    entry->s = 0;                // Must be 0 for gates
    entry->dpl = dpl;            // Descriptor Privilege Level
    entry->p = 1;                // Present
    entry->offset_high = (base >> 16) & 0xFFFF;
}
void isr_install_handler(int isr_num, isr_t handler){
    
}
void isr_uninstall_handler(int isr_num){

}
void load_idt(idt_desc *idtr){
    idtr->limit = 65535;
    idtr->base = (uint32_t)IDT_ADDR;
    asm volatile ("lidt %0" : : "m" (*idtr));
}

void pic_init(void) {
    disable_interrupts();
    // Mask all interrupts on both PICs
    outb(MASTER_PIC_DATA, 0xFF);
    outb(SLAVE_PIC_DATA, 0xFF);

    // Initialize the Master PIC
    outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);   // Send initialization command
    outb(MASTER_PIC_DATA, MASTER_PIC_VECTOR_OFFSET);   // Set the vector offset for the Master PIC (0x20)
    outb(MASTER_PIC_DATA, MASTER_PIC_ICW3);   // Configure the Master PIC to cascade with the Slave PIC (IRQ2)
    outb(MASTER_PIC_DATA, ICW4_8086);   // Set the Master PIC to 8086 mode

    // Initialize the Slave PIC
    outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);   // Send initialization command
    outb(SLAVE_PIC_DATA, SLAVE_PIC_VECTOR_OFFSET);   // Set the vector offset for the Slave PIC (0x28)
    outb(SLAVE_PIC_DATA, SLAVE_PIC_ICW3);   // Configure the Slave PIC cascade identity (connected to Master PIC IRQ2)
    outb(SLAVE_PIC_DATA, ICW4_8086);   // Set the Slave PIC to 8086 mode
    
    // Unmask interrupts after initialization
    outb(MASTER_PIC_DATA, 0xFD); // Unmask IRQ 1 (keyboard)
    outb(SLAVE_PIC_DATA, 0xFF);  // Mask all IRQs on Slave PIC
    enable_interrupts();
}

void pic_send_eoi(uint8_t irq_num){
    if(irq_num >= 8)
		outb(SLAVE_PIC_COMMAND,PIC_EOI);
	
	outb(MASTER_PIC_COMMAND,PIC_EOI);
}

void pic_set_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        irq_line -= 8;
    }
    value = inb(port) | (1 << irq_line);
    outb(port, value);        
}

void pic_clear_mask(uint8_t irq_line) {
    uint16_t port;
    uint8_t value;

    if(irq_line < 8) {
        port = MASTER_PIC_DATA;
    } else {
        port = SLAVE_PIC_DATA;
        irq_line -= 8;
    }
    value = inb(port) & ~(1 << irq_line);
    outb(port, value);        
}

//Dynamic remap of pic
void pic_remap(int offset1, int offset2){
    uint8_t a1, a2;
	
	a1 = inb(MASTER_PIC_DATA);                        // save masks
	a2 = inb(SLAVE_PIC_DATA);
	
	outb(MASTER_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(SLAVE_PIC_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(MASTER_PIC_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(SLAVE_PIC_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(MASTER_PIC_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(SLAVE_PIC_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
	
	outb(MASTER_PIC_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(SLAVE_PIC_DATA, ICW4_8086);
	io_wait();
	
	outb(MASTER_PIC_DATA, a1);   // restore saved masks.
	outb(SLAVE_PIC_DATA, a2);
}

/* Helper func */
static uint16_t pic_get_irq_reg(int OCW3)
{
    /* OCW3 to PIC CMD to get the register values.  SLAVE_PIC is chained, and
     * represents IRQs 8-15.  MASTER_PIC is IRQs 0-7, with 2 being the chain */
    outb(MASTER_PIC_COMMAND, OCW3);
    outb(SLAVE_PIC_COMMAND, OCW3);
    return (inb(SLAVE_PIC_COMMAND) << 8) | inb(MASTER_PIC_COMMAND);
}

/* Returns the combined value of the cascaded PICs irq request register */
uint16_t pic_get_irr(void)
{
    return pic_get_irq_reg(PIC_READ_IRR);
}

/* Returns the combined value of the cascaded PICs in-service register */
uint16_t pic_get_isr(void)
{
    return pic_get_irq_reg(PIC_READ_ISR);
}

//Interrupt serive routine

isr_t interrupt_handler(uint32_t interrupt_num, uint32_t error_code){
    switch (interrupt_num)
    {
    case 0:
        kprint("U suck!");
        pic_send_eoi(0);
        break;

    case 33:
        keyboard_handler();
        break;
    
    default:
        kprint("Unimplemented interrupt detected");
        pic_send_eoi(0);
        break;
    }
}

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_BUFFER_EMPTY 0x01
isr_t keyboard_handler() {
    uint8_t scan_code;
    kprint("    Key pressed.");
    // process all scan code 
    while ((inb(0x64) & 0x01)) { // check cache
        scan_code = inb(KEYBOARD_DATA_PORT); // read from data port

        // convert
        char str[3];
        str[0] = "0123456789ABCDEF"[(scan_code >> 4) & 0x0F];
        str[1] = "0123456789ABCDEF"[scan_code & 0x0F];
        str[2] = '\0';

        // print
        kprint(str);
    }

    // signal
    pic_send_eoi(1);
}
