#include "types.h"

//Necessary definitions for paging
#define PAGE_SIZE 4096 // 4 KiB
//According to Intel, a pt/pd is a page itself. So maximum 4096/4 = 1024.
#define MAX_ENTRIES 1024
#define KERNEL_PAGE_DIRECTORY_ADDR 0x00200000 //where page directory starts. Suppose we have 64M memory.
#define KERNEL_PAGE_TABLE_ADDR 0x00201000 //where page table starts

typedef struct  __attribute__((packed)) {
    unsigned int present : 1;         // present bit    
    unsigned int read_write : 1;      // read/write
    unsigned int user_supervisor : 1; // user/supervisor
    unsigned int reserved_1 : 2;      // reserved
    unsigned int accessed : 1;        // accessed bit
    unsigned int dirty : 1;           // dirty bit
    unsigned int reserved_2 : 2;      // reserved
    unsigned int available : 3;       // available for system programmer use
    unsigned int frame : 20;          // Physical frame number
} page_table_entry;

typedef struct __attribute__((packed)){
    page_table_entry pte[MAX_ENTRIES];
} page_table;

typedef struct __attribute__((packed)){
    unsigned int present : 1;         // present bit    
    unsigned int read_write : 1;      // read/write
    unsigned int user_supervisor : 1; // user/supervisor
    unsigned int reserved_1 : 2;      // reserved, PWT & PCD
    unsigned int accessed : 1;        // accessed bit
    unsigned int reserved_2 : 1;      // reserved, AVL
    unsigned int page_size : 1;       // default to 0, 4KiB ps
    unsigned int reserved_3 : 4;      // reserved, 8, 9-11
    unsigned int frame : 20;          // Physical frame number
} page_directory_entry;

typedef struct __attribute__((packed)){
    page_directory_entry pde[MAX_ENTRIES];
} page_directory;

void kinit_paging();
void kenable_paging();
void page_frame_allocate();