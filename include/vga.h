#include "types.h"
#include "x86.h"

#define VIDEO_MEM_ADDR 0xB8000
#define VGA_REG_GRAPHICS_INDEX 0x03C4
#define VGA_REG_GRAPHICS_DATA 0x03C5
#define VGA_REG_SET_MODE 0x00

// enum for foreground and backgroud
//Foreground color the last bit is bright bit.
//Background color the last bit is blink bit.
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color_t;

typedef struct {
    vga_color_t foregroud_color:4;
    vga_color_t background_color:4;
} vga_attr;

typedef struct {
    uint8_t character;
    vga_attr attr;
} vga_cell;


//vga display modes
typedef enum {
    VGA_MODE_TEXT = 0x03,      // 80x25 text mode
    VGA_MODE_GRAPHICS = 0x13,  // 320x200 graphics mode
} vga_mode_t;

//functions
void vga_set_mode(vga_mode_t);
void vga_write_mem(char*, vga_attr);
void vga_clear_screen();
