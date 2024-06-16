#include "idt_init.h"


/* Initialize the Interrupt Descriptor Table (IDT)
 * Inputs: None
 * Outputs: None
 * Function: Initializes the Interrupt Descriptor Table (IDT) entries with appropriate values,
 *           sets up exception handlers, and configures the IDT for interrupt handling. */
void initialize_idt() {
int i;

    for (i = 0; i < NUM_VEC; i++){
       // Set the segment selector for the IDT entry to KERNEL_CS.
        idt[i].seg_selector = KERNEL_CS;

        // Set various reserved and control bits for the IDT entry.
        idt[i].reserved4 = 0;   // Reserved, set to 0.
        idt[i].reserved3 = 1;   // Reserved, set to 1.
        idt[i].reserved2 = 1;   // Reserved, set to 1.
        idt[i].reserved1 = 1;   // Reserved, set to 1.
        idt[i].size = 1;        // Size of the gate (1 for 32-bit).
        idt[i].reserved0 = 0;   // Reserved, set to 0.
        idt[i].dpl = 0;         // Descriptor Privilege Level, set to 0 (kernel mode).
        idt[i].present = 0;     // Present bit, set to 0 (will be set to 1 for specific entries).

        if (i >= 0x20 && i <= 0x2F){        //PIC pins between  32-47 
            idt[i].reserved3 = 0;           // pics
        }

        if (i == 0x80){                     // 0x80 reserved for system calls 
            idt[i].dpl = 3;                 // syscall
            idt[i].present = 1;             // set present to 1
        }

        if (i >= 0x0 && i <= 0x2F){         // 0 - 32 for exceptions 
            idt[i].present = 1;             //present to 1 
        }

    }

    SET_IDT_ENTRY(idt[0], division_error_handler_asm);
    SET_IDT_ENTRY(idt[1], debug_handler_asm);
    SET_IDT_ENTRY(idt[2], non_maskable_interrupt_handler_asm);
    SET_IDT_ENTRY(idt[3], breakpoint_handler_asm);
    SET_IDT_ENTRY(idt[4], overflow_handler_asm);
    SET_IDT_ENTRY(idt[5], bound_range_exceeded_handler_asm);
    SET_IDT_ENTRY(idt[6], invalid_opcode_handler_asm);
    SET_IDT_ENTRY(idt[7], device_not_available_handler_asm);
    SET_IDT_ENTRY(idt[8], double_fault_handler_asm);

    //All exceptions need to be offset by 1 bc 9 is reserved
    SET_IDT_ENTRY(idt[10], invalid_tss_handler_asm);
    SET_IDT_ENTRY(idt[11], segment_not_present_handler_asm);
    SET_IDT_ENTRY(idt[12], stack_segment_fault_handler_asm);
    SET_IDT_ENTRY(idt[13], general_protection_fault_handler_asm);
    SET_IDT_ENTRY(idt[14], page_fault_handler_asm);
    SET_IDT_ENTRY(idt[16], x87_floating_point_exception_handler_asm);
    SET_IDT_ENTRY(idt[17], alignment_check_handler_asm);
    SET_IDT_ENTRY(idt[18], machine_check_handler_asm);
    SET_IDT_ENTRY(idt[19], SIMD_floating_point_exception_handler_asm);
    SET_IDT_ENTRY(idt[20], virtualization_exception_asm);
    SET_IDT_ENTRY(idt[21], control_protection_exception_handler_asm);
    SET_IDT_ENTRY(idt[28], hypervisor_exception_handler_asm);
    SET_IDT_ENTRY(idt[29], vmm_communication_exception_handler_asm);
    SET_IDT_ENTRY(idt[30], security_exception_handler_asm);
    ////////////////////////////////////////////////////////////////

    SET_IDT_ENTRY(idt[32], pit_handler_asm); //pit
    SET_IDT_ENTRY(idt[40], rtc_interrupt_asm); //rtc
    SET_IDT_ENTRY(idt[33], keyboard_interrupt_asm); //keyboard
     SET_IDT_ENTRY(idt[128], system_call_asm); //system call 

}








