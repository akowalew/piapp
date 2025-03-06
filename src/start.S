#include "start.h"

.section ".vectors"

.balign 0x800
vector_table:

curr_el_sp0_sync: // The exception handler for the synchronous
	b curr_el_sp0_sync // exception from the current EL using SP0.

.balign 0x80
curr_el_sp0_irq: // The exception handler for the IRQ exception
	b curr_el_sp0_irq // from the current EL using SP0.

.balign 0x80
curr_el_sp0_fiq: // The exception handler for the FIQ exception
	b curr_el_sp0_fiq // from the current EL using SP0.

.balign 0x80
curr_el_sp0_serror: // The exception handler for the system error
	b curr_el_sp0_serror // exception from the current EL using SP0.


.balign 0x80
curr_el_spx_sync: // The exception handler for the synchronous
	b curr_el_spx_sync // exception from the current EL using the

 // current SP.
.balign 0x80
curr_el_spx_irq: // The exception handler for IRQ exception
	b curr_el_spx_irq // from the current EL using the current SP.

.balign 0x80
curr_el_spx_fiq: // The exception handler for the FIQ exception
	b curr_el_spx_fiq // from the current EL using the current SP.

.balign 0x80
curr_el_spx_serror: // The exception handler for the system error
	b curr_el_spx_serror // exception from the current EL using the


 // current SP.
.balign 0x80
lower_el_aarch64_sync: // The exception handler for the synchronous
	b lower_el_aarch64_sync // exception from a lower EL (AArch64).

.balign 0x80
lower_el_aarch64_irq: // The exception handler for the IRQ exception
	b lower_el_aarch64_irq // from a lower EL (AArch64).

.balign 0x80
lower_el_aarch64_fiq: // The exception handler for the FIQ exception
	b lower_el_aarch64_fiq // from a lower EL (AArch64).

.balign 0x80
lower_el_aarch64_serror: // The exception handler for the system error
	b lower_el_aarch64_serror // exception from a lower EL(AArch64).


.balign 0x80
lower_el_aarch32_sync: // The exception handler for the synchronous
	b lower_el_aarch32_sync // exception from a lower EL(AArch32).

.balign 0x80
lower_el_aarch32_irq: // The exception handler for the IRQ exception
	b lower_el_aarch32_irq // from a lower EL (AArch32).

.balign 0x80
lower_el_aarch32_fiq: // The exception handler for the FIQ exception
	b lower_el_aarch32_fiq // from a lower EL (AArch32).

.balign 0x80
lower_el_aarch32_serror: // The exception handler for the system error
	b lower_el_aarch32_serror // exception from a lower EL(AArch32).

.section ".boot"

.global _start

_start:
	MOV X0, XZR
	MOV X1, XZR
	MOV X2, XZR

	// read cpu id, stop slave cores
	mrs		x1, mpidr_el1
	and		x1, x1, #3
	cbz 	x1, 2f

1:	// cpu id > 0

	// do nothing
	wfe
	b 		1b

2:	// cpu id == 0

	mrs x0, CurrentEL   // Read CurrentEL register into x0
    lsr x0, x0, #2

	// ldr 	x1, = vector_table
	// MSR 	VBAR_EL3, x1

	ldr 	x1, = vector_table
	MSR 	VBAR_EL2, x1

	ldr 	x1, = vector_table
	MSR 	VBAR_EL1, x1

	// MSR CPTR_EL3, XZR
	MSR CPTR_EL2, XZR  // Instructions are not trapped below EL2
	MOV X0, #0x00300000  // No trap to all NEON & FP instructions
	MSR CPACR_EL1, X0

	// set top of stack just before our code
	ldr		x1, =_start
	mov 	sp, x1

	// clear bss
	ldr		x1, =__bss_start
	ldr		w2, =__bss_size
3:	cbz		w2, 4f
	str 	xzr, [x1], #8
	sub 	w2, w2, #1
	cbnz	w2, 3b

	// jump to c code, should not return
4:	bl 		main
	b 		1b
