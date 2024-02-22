#include "stdio.h"
#include "stdbool.h"
#include "Kernel.h"

void User_task0(){
    uint32_t sp = 0;
    while (true){
        debug_printf("User Task #0 SP=0x%x\n", &sp);
        Kernel_yield();
    }
}
