// #include "keyboard.h"

// void init_keyboard(){

// }

// char translate_scan_code(uint8_t scan_code) {
//     if (scan_code >= 0x1E && scan_code <= 0x7E) {
//         return scan_code - 0x1E + 'a'; 
//     }
//     return 0; 
// }

// void keyboard_handler(){
//     uint8_t scan_code = inb(KEYBOARD_PORT);
//     char character = translate_scan_code(scan_code);
//     if(character) kprint(&character);
// }
