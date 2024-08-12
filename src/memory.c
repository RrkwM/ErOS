#include "memory.h"
#include "utils.h"

void kinit_paging() {
    kprint("Initializing paging...");
    //allocate addr for page directory and page table
    page_directory *kernel_pd = (page_directory *)KERNEL_PAGE_DIRECTORY_ADDR;

    // page directory init
    // for each entry in kernel_page_directory, do:
    // allocate pointer to page table; initilize the page table;(1024 times)
    // page table init
    // for each entry in kernel_page_table, do:
    // initialize the page_table_entry struct(1024 times, total 1M times)
    // the key difference are that pd stores pointers while pt stores struct (though the size are both 4B)
    for (int i = 0; i < MAX_ENTRIES; i++) {
        page_table *kernel_pt = (page_table *)KERNEL_PAGE_TABLE_ADDR+i*sizeof(page_table);
        kernel_pd->pt[i] = kernel_pt; 
        // page table init
        for (int j = 0; j < MAX_ENTRIES; j++) {
            kernel_pt->pte[j].reserved_1 = 0;
            kernel_pt->pte[j].accessed = 0;
            kernel_pt->pte[j].dirty = 0;
            kernel_pt->pte[j].reserved_2 = 0;
            kernel_pt->pte[j].available = 0;
            // Direct mapping setup, kerenl and BIOS area
            if (((i * MAX_ENTRIES + j) * 4096 >= KERNEL_START_ADDR) && 
                ((i * MAX_ENTRIES + j) * 4096 < KERNEL_START_ADDR + KERNEL_SIZE)
                || ((i * MAX_ENTRIES + j) * 4096 < BIOS_SIZE)) {
                // Set up direct mapping for high 8 MiB
                kernel_pt->pte[j].present = 1;
                kernel_pt->pte[j].read_write = 1;
                kernel_pt->pte[j].user_supervisor = 0;
                kernel_pt->pte[j].frame = (i * MAX_ENTRIES + j) - (KERNEL_START_ADDR / 4096);
            } else {
                // Initialize other entries as needed
                kernel_pt->pte[j].present = 0;
                kernel_pt->pte[j].read_write = 0;
                kernel_pt->pte[j].user_supervisor = 0;
                kernel_pt->pte[j].frame = 0;
            }
        }
        kprint("    Finished.");
        kprint_num(i);
    }
    kprint("Initialization Succeeded!");
}

void kenable_paging(){
    kprint("Enabling paging...");
    uint32_t kernel_pd_addr = KERNEL_PAGE_DIRECTORY_ADDR; // physical address for page directory;

    // set cr3 with page directory address
    asm volatile ("mov %0, %%cr3" : : "r"(kernel_pd_addr));

    // enable paging 
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // set paging bit
    asm volatile ("mov %0, %%cr0" : : "r"(cr0));
    kprint("Paging turned on!");
}

