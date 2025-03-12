.global _start            @ Make _start available to the linker
.section .text

_start:
    b reset_handler      @ Branch to reset handler

reset_handler:
    mrs r0, cpsr         @ Read the current processor state
    bic r0, r0, #0x1F    @ Clear the mode bits
    orr r0, r0, #0x13    @ Set CPU mode to SVC (Supervisor)
    msr cpsr, r0         @ Write back the updated mode

    ldr sp, =stack_top   @ Set stack pointer to top of the stack

    bl main       @ Call the main C function

hang:
    b hang               @ Infinite loop to prevent CPU from running off
