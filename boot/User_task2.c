#include "stdio.h"
#include "stdbool.h"
#include "Kernel.h"

void User_task2(){
    uint32_t sp = 0;

    while (true){
        debug_printf("User Task #2 SP=0x%x\n", &sp);
        Kernel_yield();
    }
}