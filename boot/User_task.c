#include "stdio.h"
#include "stdbool.h"
#include "Kernel.h"

void event_handler(){
    debug_printf("\nUart Event Handled\n");
}

void User_task0(){
    uint32_t sp = 0;
    debug_printf("User Task #0 SP=0x%x\n", &sp);
    while (true){
        
        //Connect uart event with event_handler
        KernelEventFlag_t handle_event = Kernel_wait_events(KernelEventFlag_UartIn);
        switch (handle_event)
        {
        case KernelEventFlag_UartIn:
            event_handler();
            break;
        }

        Kernel_yield();
    }
}
