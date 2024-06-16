#include "i8259.h"
#include "lib.h"
#include "idt_handlers.h"
#include "cursor.h"
#include "file_system.h"
#include "paging_asm.h"
#include "System_Calls.h"


/* Signal handler function for Division Error Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Division Error Exception and display an error message. */
void division_error_handler() {
        printf("Division Error Exception Caught!\n");
        while(1);
}

/* Signal handler function for Debug Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Debug Exception and display an error message. */
void debug_handler() {
    printf("Debug Exception Caught!\n");
    while (1);
}

/* Signal handler function for Non-Maskable Interrupt Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Non-Maskable Interrupt Exception and display an error message. */
void non_maskable_interrupt_handler() {
    printf("Non-Maskable Interrupt Exception Caught!\n");
    while (1);
}

/* Signal handler function for Breakpoint Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Breakpoint Exception and display an error message. */
void breakpoint_handler() {
    printf("Breakpoint Exception Caught!\n");
    while (1);
}

/* Signal handler function for Overflow Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Overflow Exception and display an error message. */
void overflow_handler() {
    printf("Overflow Exception Caught!\n");
    while (1);
}

/* Signal handler function for Bound Range Exceeded Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Bound Range Exceeded Exception and display an error message. */
void bound_range_exceeded_handler() {
    printf("Bound Range Exceeded Exception Caught!\n");
    while (1);
}

/* Signal handler function for Invalid Opcode Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Invalid Opcode Exception and display an error message. */
void invalid_opcode_handler() {
    printf("Invalid Opcode Exception Caught!\n");
    while (1);
}

/* Signal handler function for Device Not Available Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Device Not Available Exception and display an error message. */
void device_not_available_handler() {
    printf("Device Not Available Exception Caught!\n");
    while (1);
}

/* Signal handler function for Double Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Double Fault Exception and display an error message. */
void double_fault_handler() {
    printf("Double Fault Exception Caught!\n");
    while (1);
}

/* Signal handler function for Invalid TSS (Task State Segment) Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Invalid TSS Exception and display an error message. */
void invalid_tss_handler() {
    printf("Invalid TSS Exception Caught!\n");
    while (1);
}

/* Signal handler function for Segment Not Present Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Segment Not Present Exception and display an error message. */
void segment_not_present_handler() {
    printf("Segment Not Present Exception Caught!\n");
    while (1);
}

/* Signal handler function for Stack-Segment Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Stack-Segment Fault Exception and display an error message. */
void stack_segment_fault_handler() {
    printf("Stack-Segment Fault Exception Caught!\n");
    while (1);
}

/* Signal handler function for General Protection Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the General Protection Fault Exception and display an error message. */
void general_protection_fault_handler() {
    printf("General Protection Fault Exception Caught!\n");
    //while (1);
     halt((uint8_t)256);
    //while (1);
}

/* Signal handler function for Page Fault Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Page Fault Exception and display an error message. */
void page_fault_handler() {
    printf("Page Fault Exception Caught!\n");
    halt((uint8_t)256);
    //while (1);
}

/* Signal handler function for x87 Floating-Point Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the x87 Floating-Point Exception and display an error message. */
void x87_floating_point_exception_handler() {
    printf("x87 Floating-Point Exception Caught!\n");
    while (1);
}

/* Signal handler function for Alignment Check Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Alignment Check Exception and display an error message. */
void alignment_check_handler() {
    printf("Alignment Check Exception Caught!\n");
    while (1);
}

/* Signal handler function for Machine Check Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Machine Check Exception and display an error message. */
void machine_check_handler() {
    printf("Machine Check Exception Caught!\n");
    while (1);
}

/* Signal handler function for SIMD Floating-Point Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the SIMD Floating-Point Exception and display an error message. */
void SIMD_floating_point_exception_handler() {
    printf("SIMD Floating-Point Exception Caught!\n");
    while (1);
}

/* Signal handler function for Virtualization Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Virtualization Exception and display an error message. */
void virtualization_exception() {
    printf("Virtualization Exception Caught!\n");
    while (1);
}

/* Signal handler function for Control Protection Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Control Protection Exception and display an error message. */
void control_protection_exception_handler() {
    printf("Control Protection Exception Caught!\n");
    while (1);
}

/* Signal handler function for Hypervisor Injection Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Hypervisor Injection Exception and display an error message. */
void hypervisor_exception_handler() {
    printf("Hypervisor Injection Exception Caught!\n");
    while (1);
}

/* Signal handler function for VMM Communication Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the VMM Communication Exception and display an error message. */
void vmm_communication_exception_handler() {
    printf("VMM Communication Exception Caught!\n");
    while (1);
}

/* Signal handler function for Security Exception
 * Inputs: None
 * Outputs: None
 * Function: Handle the Security Exception and display an error message. */
void security_exception_handler() {
    printf("Security Exception Caught!\n");
    while (1);
}

/* System Call Handler Function
 * Inputs: None
 * Outputs: None
 * Function: Handles a system call interrupt. It prints a message and enters an infinite loop. */
// void system_call() {
//     printf("System call invoked.\n");  // Print a message indicating that a system call was invoked.
//     while (1);  // Enter an infinite loop.
// }

