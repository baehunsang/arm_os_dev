#include "stdint.h"
#include "stdio.h"

static void Hw_init(){
    Hal_uart_init();
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

void start_main(){
    Hw_init();
    banner();
    main();
}