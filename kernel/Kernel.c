#include "stdint.h"
#include "stdbool.h"

#include "Kernel.h"

void Kernel_yield(){
    Kernel_task_scheduler();
}

void Kernel_start(){
    Kernel_task_start();
}