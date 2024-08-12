#include "types.h"

//Necessary definitions for paging
#define PAGE_SIZE 4096 // 4 KiB
//According to Intel, a pt/pd is a page itself. So maximum 4096/4 = 1024.
#define MAX_ENTRIES 1
#define KERNEL_PAGE_DIRECTORY_ADDR 0xF00000 //where page directory starts. Right where kernel starts.
#define KERNEL_PAGE_TABLE_ADDR 0xF01000 //where page table starts
#define KERNEL_START_ADDR 0xF00000  // High 8 MiB starting address. Suggest we have 64M ram.
#define KERNEL_SIZE  (8 * 1024 * 1024) // 8 MiB
#define BIOS_SIZE (1024*1024)

typedef struct {
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

typedef struct{
    page_table_entry pte[MAX_ENTRIES];
} page_table;

typedef struct{
    page_table *pt[MAX_ENTRIES];
} page_directory;

void kinit_paging();
void kenable_paging();
