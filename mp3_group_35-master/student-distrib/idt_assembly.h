#ifndef _IDT_ASSEMBLY_H
#define _IDT_ASSEMBLY_H
#include "idt_handlers.h"

void division_error_handler_asm();                 // Division Error Exception Handler
void debug_handler_asm();                          // Debug Exception Handler
void non_maskable_interrupt_handler_asm();         // Non-Maskable Interrupt Exception Handler
void breakpoint_handler_asm();                     // Breakpoint Exception Handler
void overflow_handler_asm();                       // Overflow Exception Handler
void bound_range_exceeded_handler_asm();           // Bound Range Exceeded Exception Handler
void invalid_opcode_handler_asm();                 // Invalid Opcode Exception Handler
void device_not_available_handler_asm();           // Device Not Available Exception Handler
void double_fault_handler_asm();                   // Double Fault Exception Handler
void invalid_tss_handler_asm();                    // Invalid Task State Segment Exception Handler
void segment_not_present_handler_asm();            // Segment Not Present Exception Handler
void stack_segment_fault_handler_asm();            // Stack Segment Fault Exception Handler
void general_protection_fault_handler_asm();       // General Protection Fault Exception Handler
void page_fault_handler_asm();                     // Page Fault Exception Handler
void x87_floating_point_exception_handler_asm();   // x87 Floating-Point Exception Handler
void alignment_check_handler_asm();                // Alignment Check Exception Handler
void machine_check_handler_asm();                  // Machine Check Exception Handler
void SIMD_floating_point_exception_handler_asm();  // SIMD Floating-Point Exception Handler
void virtualization_exception_asm();               // Virtualization Exception Handler
void control_protection_exception_handler_asm();   // Control Protection Exception Handler
void hypervisor_exception_handler_asm();           // Hypervisor Exception Handler
void vmm_communication_exception_handler_asm();    // VMM Communication Exception Handler
void security_exception_handler_asm();              // Security Exception Handler
void rtc_interrupt_asm();                          // Real-Time Clock (RTC) Interrupt Handler
void keyboard_interrupt_asm();                     // Keyboard Interrupt Handler
void system_call_asm();                            // System Call Handler
void pit_handler_asm(); 
//void sys_call_table[];

#endif 




