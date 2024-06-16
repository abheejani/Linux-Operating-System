#ifndef _IDT_HANDLERS_H
#define _IDT_HANDLERS_H
// #include "types.h"
#include "tests.h"

//declaration for exception handlers, interrupt handelrs and system calls functions 

 void division_error_handler(void); 
 void debug_handler(void);
 void non_maskable_interrupt_handler(void);
 void breakpoint_handler(void);
 void overflow_handler(void);
 void bound_range_exceeded_handler(void);
 void invalid_opcode_handler(void);
 void device_not_available_handler(void);
 void double_fault_handler(void);
 void invalid_tss_handler(void);
 void segment_not_present_handler(void);
 void stack_segment_fault_handler(void);
 void general_protection_fault_handler(void);
 void page_fault_handler(void);
 void x87_floating_point_exception_handler(void);
 void alignment_check_handler(void);
 void machine_check_handler(void);
 void SIMD_floating_point_exception_handler(void);
 void virtualization_exception(void);
 void control_protection_exception_handler(void);
 void hypervisor_exception_handler(void);
 void vmm_communication_exception_handler(void);
 void security_exception_handler(void);

#endif /* _IDT_HANDLERS_H */




