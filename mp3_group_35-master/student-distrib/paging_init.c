#include "x86_desc.h"
#include "paging_asm.h"
#include "terminal.h"



/* Initialize paging 
 * Inputs: None
 * Outputs: None
 * Function: this funtion sets up paging my creating enteries in the PD and the PT. We have 2 entried in the directory and 1 PTE - used for video 
 * memeory and for kernel 
 *  */


void init_paging(){
    int i;

    // This is for each PDE. (2) 1 for video mem and one for kernel
    //here we are settng the respective bits in the 32 PDE to the correct values 
    //this i th create a PDE at index 0 of the page directory 
    //for(i = 0; i < 1024; i++){
        pd[0].P = 1; //set present bit 1 - meaning that the this entry exists 
        pd[0].RW = 1;
        pd[0].US = 0;
        pd[0].PWT = 0;
        pd[0].PCD = 0;
        pd[0].A =  0;
        pd[0].AVL1 = 0;
        pd[0].PS = 0;
        pd[0].AVL4 = 0;
        pd[0].bits_31_12 = ((unsigned int)pt>>12);//this is to a put the address of the the page table into the PDE 
    //  (page_table_t*)[0] = 0xB8000; 


//this is to create an entry of the PD at index 1 for the kernel space 
    pd[1].P = 1; //set present bit 1 - meaning that the this entry exists 
    pd[1].RW = 1;
    pd[1].US = 0;                   //urw for kernel
    pd[1].PWT = 0;
    pd[1].PCD = 0;
    pd[1].A=  0;
    pd[1].AVL1 = 0;
    pd[1].PS = 1; //set that the size should be 4MB 
    pd[1].AVL4 = 1; //since we're using the same 32 bits as when ps 0, we need to set the LSB of AVL4 to be 1, this sets the G bit high for flushing CR3 
    pd[1].bits_31_12 = 1024;//points to the pages in the kernel - we "skip" the PTE step 

    // This is for each video mem (PTE)
    for(i = 0; i < 1024; i++){ // each PTE - this is to create an entire 
        pt[i].P = 0;
        pt[i].RW = 0;
        pt[i].US = 0;
        pt[i].PWT = 0;
        pt[i].PCD = 0;
        pt[i].A = 0;
        pt[i].D = 0;
        pt[i].PAT = 0;
        pt[i].G = 0;
        pt[i].AVL = 0;
        pt[i].bits_31_12 = 0; // POST SHIFTING (i * 4096 >> 12)

            if(i==184 || i == 185 || i == 186 || i == 187){ //we only want the entry at the video memory to be present - 
                 pt[i].bits_31_12 = i; // this is the addr for the page in video mem. this is right shift by 12
                pt[i].P = 1; //making it so that only this entry is present for the PTE 
                pt[i].RW = 1;
            }

            
    }

//call for assembly linkage for setting up paging
    loadPageDirectory_asm((page_dir_t*)pd);//we need to pass by pointer I think thissl automaticalyl take mem address
}




void program_pages(uint8_t target_pid){  //, uint8_t my_boofer){

    /////////////////////////////pde for progam image////////////////////////////////////////////
    pd[32].P = 1; 
    pd[32].RW = 1;
    pd[32].US = 1;                   
    pd[32].PWT = 0;
    pd[32].PCD = 0;
    pd[32].A=  0;
    pd[32].AVL1 = 0;
    pd[32].PS = 1; //set that the size should be 4MB 
    pd[32].AVL4 = 0; //???????since we're using the same 32 bits as when ps 0, we need to set the LSB of AVL4 to be 1, this sets the G bit high for flushing CR3 
    pd[32].bits_31_12 = 2048+target_pid*1024;//points to the pages in the kernel - we "skip" the PTE step 
         
         
         asm volatile (" \
   movl %%cr3, %%eax ; \
   movl %%eax, %%cr3 ; \
   " : /* no outputs */ \
   : /* no inputs */ \
   : "eax", "cc");

 
        return;
}

void create_new_pte_for_video_memory(uint8_t** screen_start) {
    int i;
   // PDE for the 4KB page at 132MB
        pd[33].P = 1; //set present bit 1 - meaning that the this entry exists 
        pd[33].RW = 1;
        pd[33].US = 1;
        pd[33].PWT = 0;
        pd[33].PCD = 1;
        pd[33].A =  0;
        pd[33].AVL1 = 0;
        pd[33].PS = 0;
        pd[33].AVL4 = 0;
        pd[33].bits_31_12 = ((unsigned int)vid_mem>>12);//this is to a put the address of the the page table into the PDE 
     

        // This is for each video mem (PTE)
    for(i = 0; i < 1024; i++){ // each PTE - this is to create an entire 
        vid_mem[i].P = 0;
        vid_mem[i].RW = 0;
        vid_mem[i].US = 0;
        vid_mem[i].PWT = 0;
        vid_mem[i].PCD = 1;
        vid_mem[i].A = 0;
        vid_mem[i].D = 0;
        vid_mem[i].PAT = 0;
        vid_mem[i].G = 0;
        vid_mem[i].AVL = 0;
        vid_mem[i].bits_31_12 = 0; // POST SHIFTING (i * 4096 >> 12)


    if(i == 0){
    vid_mem[i].bits_31_12 = 0xB8000>>12; // this is the addr for the page in video mem. this is right shift by 12
    vid_mem[i].P = 1; //making it so that only this entry is present for the PTE
    vid_mem[i].US = 1;
    vid_mem[i].RW = 1; 
    }
    
    
    asm volatile (" \
   movl %%cr3, %%eax ; \
   movl %%eax, %%cr3 ; \
   " : /* no outputs */ \
   : /* no inputs */ \
   : "eax", "cc");       

    }
    return;

}


// void switch_terminal_page(){
    
// }

