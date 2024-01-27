#include "stdint.h"
#include "HalUart.h"
#include "stdio.h"

static void Hw_init();

void main(){
    Hw_init();

    uint32_t i = 100;
    while(i--){
        Hal_uart_put_char('N');
    }
    Hal_uart_put_char('\n');
    putstr("\n");

    putstr("Hello world\n");
}

static void Hw_init(){
    Hal_uart_init();
}