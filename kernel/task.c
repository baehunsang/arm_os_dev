#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t sTask_list[MAX_TASK_NUM];

//next task id(pid)
static uint32_t sAllocated_tcb_index;

void Kernel_task_init(){
    sAllocated_tcb_index = 0;
    /*
        +--------------------------------+
        |          padding(4byte)        |
        +--------------------------------+ 
        |            ctx->spsr           |
        +--------------------------------+
        |            ctx -> r0_r12       |
        +--------------------------------+
        |            ctx->pc             |
        +--------------------------------+<-ctx, sp(stack_top)
        |                                |
        |      task stack(resource)      |
            
        |                                |
        +--------------------------------+<-stack_base
    */
    for(uint32_t i=0; i< MAX_TASK_NUM; i++){
        sTask_list[i].stack_base = (uint8_t*)(TASK_STACK_START +(i*USR_TASK_STACK_SIZE));
        sTask_list[i].sp = (uint32_t)sTask_list[i].stack_base + USR_TASK_STACK_SIZE - 4;
        sTask_list[i].sp -= sizeof(KernelTaskContext_t);
        KernelTaskContext_t* ctx = (KernelTaskContext_t*)sTask_list[i].sp;
        ctx->pc = 0;
        ctx ->spsr = ARM_MODE_BIT_SYS;
    }
}

uint32_t Kernel_task_create(KernelTaskFunc_t startFunc){
    KernelTcb_t* newtcb = &sTask_list[sAllocated_tcb_index++];
    if(sAllocated_tcb_index > MAX_TASK_NUM){
        return NOT_ENOUGH_TASK_NUM;
    }

    KernelTaskContext_t* ctx = (KernelTaskContext_t*)newtcb->sp;
    ctx->pc = (uint32_t)startFunc;
    return (sAllocated_tcb_index - 1);
}