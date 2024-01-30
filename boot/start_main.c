#include "stdint.h"
#include "stdio.h"

static void Hw_init(){
    Hal_uart_init();
}

void banner(){
    char* str = "printf_test_str_in_stack";
    uint32_t i = 5;
    putchar('\n');
    for(uint32_t i=0; i< 100; i++){
        putchar('#');
    }
    putstr("\nnavilos Boot Complete\n");
    for(uint32_t i=0; i< 100; i++){
        putchar('#');
    }
    putstr("\n\n");

    debug_printf("################# printf test #####################\n");
    debug_printf("%s\n", "hello printf");
    debug_printf("%s\n", str);
    debug_printf("%s\n", 0);
    debug_printf("%u == 5 \n", i);
    debug_printf("%x == 0xff, %u == 255\n", 0xff, 0xff);
    debug_printf("%u\n", 0);


}

void start_main(){
    Hw_init();
    banner();
    main();
}