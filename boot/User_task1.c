#include "stdio.h"
#include "stdbool.h"
#include "Kernel.h"

void User_task1(){
    uint32_t sp = 0;
    debug_printf("User Task #1 SP=0x%x\n", &sp);
    while (true){
        Kernel_yield();
    }
}