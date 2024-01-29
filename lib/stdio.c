#include "stdio.h"
#include "HalUart.h"
#include "stdint.h"

uint32_t putstr(const char* s){
    uint32_t c = 0;
    while (*s)
    {
        Hal_uart_put_char(*s++);
        c++;
    }
    return c;
}

void putchar(const char c){
    Hal_uart_put_char(c);
}

uint8_t getchar(){
    return Hal_uart_get_char();
}

uint32_t getstr(char* s, uint32_t len){
    uint32_t c = 0;
    uint8_t tmp;
    while(c < len){
        tmp = Hal_uart_get_char();

        //echo
        Hal_uart_put_char(tmp);

        if(!tmp || !(tmp - 0x0d)){
            *(s + c) = 0x00;
            return c;
        }
        *(s + c) = tmp;
        c++;
    }
    return c;
}