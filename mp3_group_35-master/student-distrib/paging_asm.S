#define ASM     1


.globl loadPageDirectory_asm 
.globl enablePaging_asm
.globl loadPSE


/* Initialze registers and allow paging 
 * Inputs: pointer to the page directory 
 * Outputs: None
 * Function: initializes CR3 to point to PD, to enable paging and to allow for 4MiB pages 
 */

loadPageDirectory_asm:
    #stack setup and teardown 
    pushl %ebp
    movl %esp, %ebp
    movl 8(%esp), %eax #put the pointer to the PD into eax 
    movl %eax, %cr3 #now CR3 holds pointer to PD 

#this is to allow 4MiB pages to be used for when we have to do paging for the kernel 
    movl %cr4, %eax 
    orl  $0x00000010, %eax #enables the bit in the register 
    movl %eax, %cr4

#this is to finally enable paging 
    movl %cr0, %eax
    orl $0x80000001, %eax #set the 32nd bit high in the register to allow paging 
    movl %eax, %cr0


#stack teardown 

    movl %ebp, %esp


    popl %ebp
    ret

