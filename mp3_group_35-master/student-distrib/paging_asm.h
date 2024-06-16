#ifndef _PAGING_ASM_H
#define _PAGING_ASM_H

#include "types.h"



/* A page directory struct (stores 1024 pdes) */
//this one is used for both when the size is 4MB AND when the size is 4kB
typedef struct page_dir_0 {
        uint32_t P : 1; //the present bit 
        uint32_t RW : 1; //the read/write bit
        uint32_t US : 1; //user/supervisor bit 
        uint32_t PWT : 1; //write through bit 
        uint32_t PCD : 1; //cache disable bit 
        uint32_t A:  1; //accessed bit 
        uint32_t AVL1 : 1; //available bit 
        uint32_t PS : 1; //page size 
        uint32_t AVL4 : 4; //available bit 
        uint32_t bits_31_12 : 20; //addres of page table 
} page_dir_0_t;

//used for when the size is 4MB
typedef struct page_dir_1 {
        uint32_t P : 1; //the present bit 
        uint32_t RW : 1; //the read/write bit
        uint32_t US : 1; //user/supervisor bit 
        uint32_t PWT : 1; //write through bit 
        uint32_t PCD : 1; //cache disable bit 
        uint32_t A:  1; //accessed bit 
        uint32_t D : 1; //dirty bit 
        uint32_t PS : 1; //page size 
        uint32_t G : 1; //global bit - for flushing 
        uint32_t AVL : 3; //available bit 
        uint32_t PAT : 1; //page attribute 
        uint32_t bits_39_32 : 8; //addr of page 
        uint32_t RSVD : 1; //reserved 
        uint32_t bits_31_22 :10; //addr of page 
} page_dir_1_t;

//this is to create a union of the two structs since theres 2 PDE 32-bit entries 
typedef union page_dir_t{
    page_dir_0_t dir_0; //for when size is 0 
    page_dir_1_t dir_1; //for when size is 1 
} page_dir_t;

/* A page table struct (stores 1024 ptes) */
    typedef struct page_table  {
        uint32_t P : 1; //the present bit 
        uint32_t RW : 1; //the read/write bit
        uint32_t US : 1; //user/supervisor bit 
        uint32_t PWT : 1; //write through bit 
        uint32_t PCD : 1; //cache disable bit 
        uint32_t A:  1; //accessed bit 
        uint32_t D : 1; //dirty bit 
        uint32_t PAT : 1; //page attribute 
        uint32_t G : 1; //global bit - for flushing 
        uint32_t AVL : 3; //available bit 
        uint32_t bits_31_12 : 20; //addr of page 
    } page_table_t;


//these lines are used to align the page directory and the page table on the 4kB boundaries 
page_dir_0_t pd[1024] __attribute__((aligned(4096))); //1024 - number of PDEs and 4096 is the number of bytes for a page 
page_table_t pt[1024] __attribute__((aligned(4096))); //1024 - number of PTEs and 4096 is the number of bytes for a page 
page_table_t vid_mem[1024] __attribute__((aligned(4096))); //1024 - number of PTEs and 4096 is the number of bytes for a page 


//linkage - this is for the assembly function so that we can enable paging and CR3 
void init_paging(); 
void loadPageDirectory_asm(page_dir_t *pd);
void program_pages(uint8_t target_pid);
void create_new_pte_for_video_memory(uint8_t** screen_start);
// void enablePaging_asm();
// void loadPSE(); 
#endif /* ASM */

//#endif /* _x86_DESC_H */





