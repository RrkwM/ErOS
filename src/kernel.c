#include "types.h"
#include "x86.h"
#include "vga.h"
#include "utils.h"
#include "memory.h"
#include "interrupt.h"

void main(){
    vga_set_mode(VGA_MODE_TEXT);
    vga_clear_screen();
    kprint("Kernel loaded.");
    kinit_paging();
    kenable_paging();
    kprint("Paging Enabled.");
    kinit_interrupt();
    kprint("                         ...halt");
    for(;;){
        __asm__ volatile ("hlt\n\t"); 
    }
}