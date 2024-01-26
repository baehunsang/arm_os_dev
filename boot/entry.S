#include "ARMv7AR.h"
#include "MemoryMap.h"

.text
    .code 32
    .global vector_start
    .global vector_end

    vector_start:
        //Exception vector table
        ldr pc, reset_handler_addr
        ldr pc, undef_handler_addr
        ldr pc, svc_handler_addr
        ldr pc, pftch_abt_handler_addr
        ldr pc, data_abt_handler_addr
        b .
        ldr pc, irq_handler_addr
        ldr pc, fiq_handler_addr

        reset_handler_addr: .word reset_handler
        undef_handler_addr: .word dummy_handler
        svc_handler_addr: .word dummy_handler
        pftch_abt_handler_addr: .word dummy_handler
        data_abt_handler_addr: .word dummy_handler
        irq_handler_addr: .word dummy_handler
        fiq_handler_addr: .word dummy_handler
        

    //vector_end:
    
    //Exception vector handler
    reset_handler:
        //svc
        mrs r0, cpsr
        bic r1, r0, #0x1f //0001 1111 r1의 하위 5bit clear
        orr r1, r1, #ARM_MODE_BIT_SVC //r1 <- ARM_MODE_BIT
        msr cpsr, r1 //모드 변경
        ldr sp, =SVC_STACK_TOP //해당 모드의 stack top으로 sp 변경
                       //(해당 mode의 banked register값 중 sp 초기화)
        //irq
        mrs r0, cpsr
        bic r1, r0, #0x1f 
        orr r1, r1, #ARM_MODE_BIT_IRQ
        msr cpsr, r1 
        ldr sp, =IRQ_STACK_TOP

        //fiq
        mrs r0, cpsr
        bic r1, r0, #0x1f 
        orr r1, r1, #ARM_MODE_BIT_FIQ 
        msr cpsr, r1 
        ldr sp, =FIQ_STACK_TOP

        //abt
        mrs r0, cpsr
        bic r1, r0, #0x1f 
        orr r1, r1, #ARM_MODE_BIT_ABT 
        msr cpsr, r1 
        ldr sp, =ABT_STACK_TOP

        //und
        mrs r0, cpsr
        bic r1, r0, #0x1f 
        orr r1, r1, #ARM_MODE_BIT_UND 
        msr cpsr, r1 
        ldr sp, =UND_STACK_TOP

        //sys
        mrs r0, cpsr
        bic r1, r0, #0x1f 
        orr r1, r1, #ARM_MODE_BIT_SYS 
        msr cpsr, r1 
        ldr sp, =USRSYS_STACK_TOP


    dummy_handler:
        b .
.end