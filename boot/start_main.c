#include "stdint.h"
#include "stdio.h"
#include "HalInterrupt.h"
#include "stdlib.h"
#include "stdbool.h"
#include "Kernel.h"

void User_task0();
void User_task1();
void User_task2();



static void Kernel_init(){
    uint32_t taskId;

    Kernel_task_init();
    taskId = Kernel_task_create(User_task0);
    if(NOT_ENOUGH_TASK_NUM == taskId){
        putstr("[ERROR]Process creation Fail\n");
    }

    taskId = Kernel_task_create(User_task1);
    if(NOT_ENOUGH_TASK_NUM == taskId){
        putstr("[ERROR]Process creation Fail\n");
    }

    taskId = Kernel_task_create(User_task2);
    if(NOT_ENOUGH_TASK_NUM == taskId){
        putstr("[ERROR]Process creation Fail\n");
    }

    Kernel_start();
}

static void Hw_init(){
    Hal_interrupt_init();
    Hal_uart_init();
    Hal_timer_init();
}

void printf_test(){
    char* str = "printf_test_str_in_stack";
    uint32_t i = 5;
    uint32_t* sysctrl0 = (uint32_t*)0x10001000;
    debug_printf("################# printf test #####################\n");
    debug_printf("%s\n", "hello printf");
    debug_printf("%s\n", str);
    debug_printf("%s\n", 0);
    debug_printf("%u == 5 \n", i);
    debug_printf("%x == 0xff, %u == 255\n", 0xff, 0xff);
    debug_printf("%u\n", 0);
    debug_printf("SYSCTRL0 %x\n", *sysctrl0);
}

void banner(){
    putchar('\n');
    for(uint32_t i=0; i< 100; i++){
        putchar('#');
    }
    putstr("\nnavilos Boot Complete\n");
    for(uint32_t i=0; i< 100; i++){
        putchar('#');
    }
    putstr("\n\n");
}

static void Timer_test(){
    while(true){
        debug_printf("current count: %u\n", Hal_timer_get_1ms_counter());
        delay(1000);
    }
}


void start_main(){
    Hw_init();
    banner();
    Kernel_init();
    //printf_test();
    //Timer_test();
    //main();
}