#include "vga.h"

//set vga mode
void vga_set_mode(vga_mode_t mode){
    outb(VGA_REG_GRAPHICS_INDEX, VGA_REG_SET_MODE);
    outb(VGA_REG_GRAPHICS_DATA, mode);
}

//write to video memory to display string
void vga_write_mem(char* str, vga_attr attr) {
    volatile uint16_t* video_memory = (uint16_t*) VIDEO_MEM_ADDR;
    
    for (int i = 0; str[i] != '\0'; i++) {
        video_memory[i] = (attr.foregroud_color << 8) | str[i];
    }
}

//fill screen with blank char
void vga_clear_screen() {
    volatile uint16_t* video_memory = (uint16_t*) VIDEO_MEM_ADDR;
    
    for (int i = 0; i<80*25; i++) {
        video_memory[i] = 0;
    }
}
