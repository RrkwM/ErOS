#include "utils.h"
#include "vga.h"

//simple encapsulation of vga_write_mem
void kprint(char* str){
    vga_attr attr = {VGA_COLOR_WHITE, VGA_COLOR_BLACK};
    vga_write_mem(str, attr);
}

void kprint_num(int num){
    vga_attr attr = {VGA_COLOR_WHITE, VGA_COLOR_BLACK};
    char str[12]; // enough for 32 bit interger with sign and '\0'
    int i = 0;
    
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
    } else {
        // negative
        if (num < 0) {
            str[i++] = '-';
            num = -num;
        }

        // num to string
        int start = i; // point to the start of num
        while (num > 0) {
            str[i++] = (num % 10) + '0';
            num /= 10;
        }
        str[i] = '\0';

        // reverse
        for (int j = start, k = i - 1; j < k; j++, k--) {
            char temp = str[j];
            str[j] = str[k];
            str[k] = temp;
        }
        vga_write_mem(str, attr);
    }
}