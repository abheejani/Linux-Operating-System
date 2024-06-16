#define ASM  1
//#include "x86_desc.h"




.globl division_error_handler_asm
.globl debug_handler_asm
.globl non_maskable_interrupt_handler_asm
.globl breakpoint_handler_asm
.globl overflow_handler_asm
.globl bound_range_exceeded_handler_asm
.globl invalid_opcode_handler_asm
.globl device_not_available_handler_asm
.globl double_fault_handler_asm
.globl invalid_tss_handler_asm
.globl segment_not_present_handler_asm
.globl stack_segment_fault_handler_asm
.globl general_protection_fault_handler_asm
.globl page_fault_handler_asm
.globl x87_floating_point_exception_handler_asm
.globl alignment_check_handler_asm
.globl machine_check_handler_asm
.globl SIMD_floating_point_exception_handler_asm
.globl virtualization_exception_asm
.globl control_protection_exception_handler_asm
.globl hypervisor_exception_handler_asm
.globl vmm_communication_exception_handler_asm
.globl security_exception_handler_asm
.globl rtc_interrupt_asm
.globl keyboard_interrupt_asm
.globl system_call_asm

.globl system_call_dispatcher #new for cp 3.3
.globl sys_call_table         #new for cp 3.3


.globl pit_handler_asm

   # Division Error Handler
division_error_handler_asm:
    pushal                              # Push all general-purpose registers (EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI)
    pushfl                              # Push the EFLAGS register (processor status flags)
    call division_error_handler          # Call the 'division_error_handler' function
    popfl                               # Pop the EFLAGS register, restoring the processor status flags
    popal                               # Pop all the general-purpose registers, restoring their values
    iret                                # Return from the interrupt, resuming normal execution

# Debug Exception Handler
debug_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call debug_handler                    # Call the 'debug_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Non-Maskable Interrupt (NMI) Handler
non_maskable_interrupt_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call non_maskable_interrupt_handler    # Call the 'non_maskable_interrupt_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Breakpoint Exception Handler
breakpoint_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call breakpoint_handler                # Call the 'breakpoint_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Overflow Exception Handler
overflow_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call overflow_handler                  # Call the 'overflow_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Bound Range Exceeded Exception Handler
bound_range_exceeded_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call bound_range_exceeded_handler      # Call the 'bound_range_exceeded_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Invalid Opcode Exception Handler
invalid_opcode_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call invalid_opcode_handler            # Call the 'invalid_opcode_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Device Not Available Exception Handler
device_not_available_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call device_not_available_handler      # Call the 'device_not_available_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Double Fault Exception Handler
double_fault_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call double_fault_handler              # Call the 'double_fault_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Invalid TSS (Task State Segment) Exception Handler
invalid_tss_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call invalid_tss_handler               # Call the 'invalid_tss_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Segment Not Present Exception Handler
segment_not_present_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call segment_not_present_handler      # Call the 'segment_not_present_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Stack Segment Fault Exception Handler
stack_segment_fault_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call stack_segment_fault_handler       # Call the 'stack_segment_fault_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# General Protection Fault Exception Handler
general_protection_fault_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call general_protection_fault_handler  # Call the 'general_protection_fault_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Page Fault Exception Handler
page_fault_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call page_fault_handler                # Call the 'page_fault_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# x87 Floating-Point Exception Handler
x87_floating_point_exception_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call x87_floating_point_exception_handler # Call the 'x87_floating_point_exception_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Alignment Check Exception Handler
alignment_check_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call alignment_check_handler           # Call the 'alignment_check_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Machine Check Exception Handler
machine_check_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call machine_check_handler            # Call the 'machine_check_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# SIMD Floating-Point Exception Handler
SIMD_floating_point_exception_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call SIMD_floating_point_exception_handler # Call the 'SIMD_floating_point_exception_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Virtualization Exception Handler
virtualization_exception_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call virtualization_exception         # Call the 'virtualization_exception' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Control Protection Exception Handler
control_protection_exception_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call control_protection_exception_handler # Call the 'control_protection_exception_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Hypervisor Exception Handler
hypervisor_exception_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call hypervisor_exception_handler      # Call the 'hypervisor_exception_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# VMM Communication Exception Handler
vmm_communication_exception_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call vmm_communication_exception_handler # Call the 'vmm_communication_exception_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Security Exception Handler
security_exception_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call security_exception_handler        # Call the 'security_exception_handler' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# RTC (Real-Time Clock) Interrupt Handler
rtc_interrupt_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call rtc_interrupt                     # Call the 'rtc_interrupt' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

# Keyboard Interrupt Handler
keyboard_interrupt_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call keyboard_interrupt                # Call the 'keyboard_interrupt' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret                                # Return from the interrupt

pit_handler_asm:
    pushal                               # Push all general-purpose registers
    pushfl                               # Push the EFLAGS register
    call pit_interrupt                # Call the 'pit_interrupt' function
    popfl                               # Pop the EFLAGS register
    popal                               # Pop all the general-purpose registers
    iret  




# System Call Handler
#handler will call dispatcher which will index into jump table
system_call_asm:
    // pushl %ecx
    // pushl %edx
    // pushl %ebx
    // pushl %esp

     cmpl $1, %eax
    jl failure
    cmpl $10, %eax 
    jg failure
     
    decl %eax

    pushl %ebp
    pushl %esi
    pushl %edi

    pushfl

    pushl %edx
    pushl %ecx
    pushl %ebx

   

    call *sys_call_table(, %eax, 4)
    addl $12, %esp

    popfl

    popl %edi
    popl %esi
    popl %ebp
    // popl %esp
    // popl %ebx
    // popl %edx
    // popl %ecx
    iret


    //addl $4, %esp #we domt wanna overwrite eax 
  

    failure: 
    movl $-1, %eax
    // addl $12, %esp  
    // popfl
    // popl %edi
    // popl %esi
    // popl %ebp
    // popl %esp
    // popl %ebx
    // popl %edx
    // popl %ecx
    iret


sys_call_table:
//.long 0
.long halt
.long execute
.long read_syscall
.long write_syscall
.long open_syscall
.long close_syscall
.long getargs
.long vidmap
.long set_handler
.long sigreturn
