#include "stdint.h"
#include "stdbool.h"
#include "memio.h"
#include "event.h"

#include "Kernel.h"

void Kernel_yield(){
    Kernel_task_scheduler();
}

void Kernel_start(){
    Kernel_task_start();
}

/*
    Add event in event list(sEventFlag)
*/
void Kernel_send_event(uint32_t event_list){
    for(uint32_t i=0; i< MAX_EVENT_NUM; i++){
        if((event_list >> i) & 1){
            KernelEventFlag_t sending_event = KernelEventFlag_Empty;
            sending_event = (KernelEventFlag_t)SET_BIT(sending_event ,i);
            Kernel_event_flag_set(sending_event);
        }
    }
}


/*
    Manage event in event list
*/
KernelEventFlag_t Kernel_wait_events(uint32_t waiting_list){
    for(uint32_t i=0; i < MAX_EVENT_NUM; i++){
        if((waiting_list >> i) & 1){
            KernelEventFlag_t waiting_event = KernelEventFlag_Empty;
            waiting_event = (KernelEventFlag_t)SET_BIT(waiting_event, i);

            if(Kernel_event_flag_check(waiting_event)){
                return waiting_event;
            }
        }
    }
    return KernelEventFlag_Empty;
}