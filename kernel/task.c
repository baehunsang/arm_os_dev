#include "stdint.h"
#include "stdbool.h"

#include "ARMv7AR.h"
#include "task.h"

static KernelTcb_t sTask_list[MAX_TASK_NUM];

//next task id(pid)
static uint32_t sAllocated_tcb_index;

static uint32_t sCurrent_tcb_index;
static KernelTcb_t* Scheduler_round_robin_algorithm();

static KernelTcb_t* sCurrent_tcb;
static KernelTcb_t* sNext_tcb;
void Kernel_task_scheduler();

__attribute__((naked)) void Kernel_task_context_switching();
static __attribute__((naked)) void Save_Context();
static __attribute__((naked)) void Restore_Context();

void Kernel_task_start();

void Kernel_task_init(){
    sAllocated_tcb_index = 0;
    sCurrent_tcb_index = 0;
    /*
        +--------------------------------+
        |          padding(4byte)        |
        +--------------------------------+ 
        |            ctx->pc             |
        +--------------------------------+
        |            ctx -> r0_r12       |
        +--------------------------------+
        |            ctx->spsr           |
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

void Kernel_task_start(){
    sNext_tcb = &sTask_list[sCurrent_tcb_index];
    Restore_Context();
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

static KernelTcb_t* Scheduler_round_robin_algorithm(){
    sCurrent_tcb_index++;
    sCurrent_tcb_index %= sAllocated_tcb_index;

    return &sTask_list[sCurrent_tcb_index];
}

void Kernel_task_scheduler(){
    sCurrent_tcb = &sTask_list[sCurrent_tcb_index];
    sNext_tcb = Scheduler_round_robin_algorithm();
    Kernel_task_context_switching();
}

__attribute__((naked)) void Kernel_task_context_switching(){
    __asm__("b Save_Context");
    __asm__("b Restore_Context");
}


    /*
        +--------------------------------+
        |          padding(4byte)        |
        +--------------------------------+ 
        |            ctx->pc             |
        +--------------------------------+
        |            ctx -> r0_r12       |
        +--------------------------------+
        |            ctx->spsr           |
        +--------------------------------+<-ctx, sp
    */
static __attribute__((naked)) void Save_Context(){
    __asm__("push {lr}");
    __asm__("push {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    //r0 = cpsr;
    __asm__("mrs r0, cpsr");
    __asm__("push {r0}");

    //save current sp to current tcb
    /*
        typedef struct KernelTcb_t
            {
                uint32_t sp; //offset: 0x0
                uint8_t* stack_base; //offset: 0x4
            } KernelTcb_t;
    */
    //r0 = &sCurrent_tcb
    __asm__("ldr r0, =sCurrent_tcb");
    //r0 = sCurrent_tcb
    __asm__("ldr r0, [r0]");
    //sCurrent_tcb->sp = sp 
    __asm__("stmia r0!, {sp}");

}

static __attribute__((naked)) void Restore_Context(){
    //sp = sNext_tcb->sp
    __asm__("ldr r0, =sNext_tcb");
    __asm__("ldr r0, [r0]");
    __asm__("ldmia r0!, {sp}");

    //restore next context from the next task stack
    __asm__("pop {r0}");
    __asm__("msr cpsr, r0");
    __asm__("pop {r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12}");
    __asm__("pop {pc}");
}