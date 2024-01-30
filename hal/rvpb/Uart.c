#include "stdint.h"
#include "HalUart.h"
#include "Uart.h"
#include "HalInterrupt.h"

extern volatile PL011_t* Uart;

static void interrupt_handler(){
    uint8_t ch = Hal_uart_get_char();
    Hal_uart_put_char(ch);
}

void Hal_uart_init(){
    Uart->uartcr.bits.UARTEN = 0;
    Uart->uartcr.bits.TXE = 1;
    Uart->uartcr.bits.RXE = 1;
    Uart->uartcr.bits.UARTEN = 1;

    //enable input interrupt
    Uart->uartimsc.bits.RXIM = 1;

    //register uart interrupt handler
    Hal_interrupt_enable(UART_INTERRUPT0);
    Hal_interrupt_register_handler(interrupt_handler, UART_INTERRUPT0);
}

void Hal_uart_put_char(uint8_t ch){
    while (Uart->uartfr.bits.TXFF);
    Uart->uartdr.all = (ch & 0xff);
}

uint8_t Hal_uart_get_char(void){
    uint32_t data;

    //RX buffer에 값이 들어올때 까지 대기
    while(Uart->uartfr.bits.RXFE);

    data = Uart->uartdr.all;

    if(data & 0xf00){
        Uart->uartrsr.all = 0xf;
        return 0;
    }

    return (uint8_t)(data & 0xff);
}
