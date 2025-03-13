.global _start
.section .boot

_start:
    b reset_handler
    b undefined_handler
    b svc_handler
    b prefetch_abort
    b data_abort
    b reserved_handler
    b irq_handler
    b fiq_handler

undefined_handler:
    b undefined_handler

svc_handler:
    b svc_handler

prefetch_abort:
    b prefetch_abort

data_abort:
    b data_abort

reserved_handler:
    b reserved_handler

irq_handler:
    b irq_handler

fiq_handler:
    b fiq_handler

reset_handler:
    ldr sp, =stack_top

zero_bss:
    ldr r0, =_sbss
    ldr r1, =_ebss
    mov r2, #0
zero_bss_loop:
    cmp r0, r1
    beq copy_data
    str r2, [r0], #4
    b zero_bss_loop

copy_data:
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
copy_data_loop:
    cmp r0, r1
    beq done_init
    ldr r3, [r2], #4
    str r3, [r0], #4
    b copy_data_loop

done_init:
    bl main

hang:
    b hang
