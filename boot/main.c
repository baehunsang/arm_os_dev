#include "stdint.h"
#include "stdio.h"

void main(){
    putstr("Hello world\n");

    for(uint32_t i=0; i<100; i++){
        uint8_t ch = Hal_uart_get_char();
        putchar(ch);
    }
}

