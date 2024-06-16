/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"


#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)     // PIC 1 data is PIC + 1 on OS DEV
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)   // PIC 2 data is PIC2 + 1 on OS DEV


#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define EOI 0x60                   //Read Write data port for ps2/keyboard controller 



/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */

/* Initialize the 8259 PIC (Programmable Interrupt Controller)
 *
 * This function initializes the 8259 PIC, which is used to manage interrupts
 * in x86-based systems. It sets the PIC to cascade mode and masks all
 * interrupts initially.
 *
 * Inputs: None
 * Outputs: None (void)
 */
void i8259_init(void) {
    // Save the current interrupt masks (not used in this code)
    // unsigned char a1, a2;
    // slave_mask = inb(PIC2_DATA);
    // master_mask = inb(PIC1_DATA);

    // Mask all interrupts (disable all interrupts)
    outb(0xFF, PIC1_DATA); // ICW2: Master PIC vector offset
    outb(0xFF, PIC2_DATA); // ICW2: Slave PIC vector offset

    // Initialize the PICs
    outb(ICW1_INIT | ICW1_ICW4, PIC1_COMMAND); // ICW1: Initialization, ICW4 needed
    outb(PIC1_COMMAND, PIC1_DATA); // ICW2: Set the vector offset
    outb(0x04, PIC1_DATA); // ICW3: Tell Master PIC that there is a slave PIC at IRQ2. 0x04 = /* Call address interval 4 (8) */
    outb(ICW4_8086, PIC1_DATA); // ICW4: 8086/88 (MCS-80/85) mode

    outb(ICW1_INIT | ICW1_ICW4, PIC2_COMMAND); // ICW1: Initialization, ICW4 needed
    outb(0x28, PIC2_DATA); // ICW2: Set the vector offset for the slave PIC. slave pic offset .Master PIC OFFSET 
    outb(0x02, PIC2_DATA); // ICW3: Tell Slave PIC its cascade identity
    outb(ICW4_8086, PIC2_DATA); // ICW4: 8086/88 (MCS-80/85) mode

    // Restore the previous interrupt masks (not used in this code)
    // outb(slave_mask, PIC2_DATA);
    // outb(master_mask, PIC1_DATA);
}


/* Enable (unmask) the specified IRQ
 *
 * This function enables (unmasks) a specified IRQ line by modifying the PIC's
 * interrupt mask register. It determines the appropriate PIC (master or slave)
 * based on the IRQ number and then updates the mask accordingly.
 *
 * Inputs:
 * - irq_num: The number of the IRQ line to enable (0-15)
 *
 * Outputs: None
 */
void enable_irq(uint32_t irq_num) {
    uint16_t port; 
    uint8_t value;
    
    // Determine the appropriate PIC and port for the specified IRQ number
    if (irq_num < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq_num -= 8;
    }

    // Update the interrupt mask by unsetting the bit corresponding to the IRQ number
    value = inb(port) & ~(1 << irq_num);
    outb(value, port);
}





/* Disable (mask) the specified IRQ
 *
 * This function disables (masks) a specified IRQ line by modifying the PIC's
 * interrupt mask register. It determines the appropriate PIC (master or slave)
 * based on the IRQ number and then updates the mask accordingly to disable the
 * IRQ.
 *
 * Inputs:
 * - irq_num: The number of the IRQ line to disable (0-15)
 *
 * Outputs: None
 */
/* Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  uint16_t port; // Variable to store the I/O port address for the PIC
    uint8_t value;// Variable to store the current value of the PIC's interrupt mask register
     
     
     // Determine the appropriate PIC and port for the specified IRQ number
    if(irq_num<8){
        port = PIC1_DATA; // If the IRQ number is less than 8, it belongs to the master PIC
    } else{
        port = PIC2_DATA; // If the IRQ number is 8 or greater, it belongs to the slave PIC
        irq_num -= 8;  // Adjust the IRQ number to account for the slave PIC
    }
    // Update the interrupt mask by setting the bit corresponding to the IRQ number
    value = inb(port) | (1 << irq_num); // Read the current value of the interrupt mask register & Set the bit corresponding to the specified IRQ to disable it
    outb(value, port); // Write the updated value back to the interrupt mask register
}



/* Send end-of-interrupt signal for the specified IRQ
 *
 * This function sends an end-of-interrupt (EOI) signal to the PIC to indicate
 * that the handling of the specified IRQ is complete. It determines whether
 * the IRQ belongs to the master or slave PIC and sends the EOI accordingly.
 *
 * Inputs:
 * - irq_num: The number of the IRQ line to send EOI for (0-15)
 *
 * Outputs: None
 */
void send_eoi(uint32_t irq_num) {
    if (irq_num < 8) { // 8 for n0-7 interrupts
        // IRQ belongs to the master PIC, send EOI to the master PIC
        outb(EOI | irq_num, PIC1_COMMAND);
    } else if (irq_num < 16) { // 0-15 interrupt 
        // IRQ belongs to the slave PIC, send EOI to the slave PIC
        outb(EOI | (irq_num - 8), PIC2_COMMAND); // 0-7 interrupt 
        // Send EOI to the master PIC to acknowledge the cascade
        outb(EOI | 2, PIC1_COMMAND);
        return; // Exit the function
    }
}



