#include "utils.h"
#include "vga.h"

//simple encapsulation of vga_write_mem
void kprint(char* str){
    vga_attr attr = {VGA_COLOR_WHITE, VGA_COLOR_BLACK};
    vga_write_mem(str, attr);
}
