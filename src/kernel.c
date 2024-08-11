#include "types.h"
#include "x86.h"
#include "vga.h"
#include "keyboard.h"
#include "utils.h"

void main(){
    char hello[] = "hello my kernel";
    vga_set_mode(VGA_MODE_TEXT);
    vga_clear_screen();
    kprint(hello);
}