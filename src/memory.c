#include "memory.h"
#include "utils.h"
#include "x86.h"

void kinit_paging() {
    //allocate addr for page directory and page table
    page_directory *kernel_pd = (page_directory *)KERNEL_PAGE_DIRECTORY_ADDR;
    
    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    unsigned int i;
    unsigned int j;
    for(j=0;j<4;j++){
        // Calculate the physical address of the page table
        unsigned int kernel_pt_physical_addr = KERNEL_PAGE_TABLE_ADDR + j * sizeof(page_table);
        // Map the page table
        page_table *kernel_pt = (page_table *)kernel_pt_physical_addr;
        //we will fill all 1024 entries in the table, mapping 4 megabytes
        for(i = 0; i < MAX_ENTRIES; i++) {
            kernel_pt->pte[i].frame = i+j*1024; // Set the frame number (physical page number)
            kernel_pt->pte[i].present = 1; // Set the present bit
            kernel_pt->pte[i].read_write = 1; // Set the read/write bit
            kernel_pt->pte[i].user_supervisor = 1; // Set the user/supervisor bit (supervisor level)
            kernel_pt->pte[i].accessed = 0; // Set the accessed bit (optional, default 0)
            kernel_pt->pte[i].dirty = 0; // Set the dirty bit (optional, default 0)
            kernel_pt->pte[i].available = 0; // Set the available bits (optional, default 0)
        }
        kernel_pd->pde[j].frame = kernel_pt_physical_addr >> 12 & 0xFFFFF;
        kernel_pd->pde[j].present = 1;
        kernel_pd->pde[j].read_write = 1;
        kernel_pd->pde[j].user_supervisor = 1;
        kernel_pd->pde[j].page_size = 0;
        kernel_pd->pde[j].accessed = 0;
    }
    kprint("Initialization Succeeded!");
}

void kenable_paging(){
    kprint("Enabling paging...");
    disable_interrupts();
    uint32_t kernel_pd_addr = KERNEL_PAGE_DIRECTORY_ADDR; // physical address for page directory;

    // set cr3 with page directory address
    asm volatile ("mov %0, %%cr3" : : "r"(kernel_pd_addr));
    // enable paging 
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile ("mov %0, %%cr0" : : "r"(cr0));
    enable_interrupts();
    kprint("Paging turned on!");
}
